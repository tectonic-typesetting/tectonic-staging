// src/main.rs -- the zip2tarindex conversion helper program
// Copyright 2017 The Tectonic Project
// Licensed under the MIT License.

extern crate clap;
extern crate flate2;
extern crate zip;

use clap::{Arg, App};
use std::{fmt, path, process};
use std::fs::File;
use std::io::{stderr, Cursor, Error, ErrorKind, Read,  Write};
use std::os::unix::ffi::OsStringExt;
use zip::ZipArchive;

// Here is stuff from tar-rs's lib.rs:

extern crate libc;
//#[cfg(all(unix, feature = "xattr"))]
//extern crate xattr;

pub use header::{Header, OldHeader, UstarHeader, GnuHeader, GnuSparseHeader};
pub use header::{GnuExtSparseHeader};
pub use entry_type::EntryType;
//pub use entry::Entry;
//pub use archive::{Archive, Entries};
pub use builder::HackedBuilder;
pub use pax::{PaxExtensions, PaxExtension};

//mod archive;
mod builder;
//mod entry;
mod entry_type;
mod error;
mod header;
mod pax;

fn other(msg: &str) -> Error {
    Error::new(ErrorKind::Other, msg)
}

// End tar-rs copy-paste.

fn die(args: fmt::Arguments) -> ! {
    writeln!(&mut stderr(), "error: {}", args).expect("write to stderr failed");
    process::exit(1)
}


fn main() {
    let matches = App::new("zip2tarindex")
        .version("0.1")
        .about("Convert a Zip file to a tar file with an index.")
        .arg(Arg::with_name("ZIPFILE")
             .help("The input Zip file to process.")
             .required(true)
             .index(1))
        .arg(Arg::with_name("TARPATH")
             .help("The name of the output tar file to create.")
             .required(true)
             .index(2))
        .get_matches();

    let zippath = matches.value_of("ZIPFILE").unwrap();
    let tarpath = matches.value_of("TARPATH").unwrap();

    // Open files.

    let zipfile = match File::open(zippath) {
        Ok(f) => f,
        Err(e) => die(format_args!("failed to open \"{}\": {}", zippath, e)),
    };

    let mut tarfile = match File::create(tarpath) {
        Ok(f) => f,
        Err(e) => die(format_args!("failed to create \"{}\": {}", tarpath, e)),
    };

    let mut indexpath = path::PathBuf::from(tarpath);
    let mut tar_fn = indexpath.file_name().unwrap().to_os_string();
    tar_fn.push(".index.gz");
    indexpath.set_file_name(&tar_fn);
    let indexfile = match File::create(&indexpath) {
        Ok(f) => f,
        Err(e) => die(format_args!("failed to create \"{}\": {}", indexpath.display(), e)),
    };

    // Stack up our I/O processing chain.

    let mut zip = match ZipArchive::new(zipfile) {
        Ok(a) => a,
        Err(e) => die(format_args!("couldn\'t open {} as a Zip file: {}", zippath, e))
    };

    if zip.len() == 0xFFFF {
        // OK, I suppose it's possible that the Zip file really has exactly
        // 65535 files in it. But zip-rs 0.2.3 doesn't handle ZIP64 files, and
        // the behavior you get is what's tested for here -- which then can
        // cause silent failures, with the resulting tar file not containing
        // all of the files it should.
        die(format_args!("this Zip file requires a ZIP64-capable parser, which we don't have"));
    }

    let mut gzindex = flate2::GzBuilder::new()
        .filename(tar_fn.into_vec())
        .write(indexfile, flate2::Compression::default());

    let mut tar = HackedBuilder::new(&mut tarfile, &mut gzindex);

    // Ready to go.

    let mut header = Header::new_gnu();

    for i in 0..zip.len() {
        let mut file = zip.by_index(i).unwrap();
        let size = file.size();

        let mut buf = Vec::with_capacity(size as usize);
        if let Err(e) = file.read_to_end(&mut buf) {
            die(format_args!("failure reading \"{}\" from Zip: {}", file.name(), e));
        }

        if let Err(e) = header.set_path(file.name()) {
            die(format_args!("failure encoding filename \"{}\": {}", file.name(), e));
        }

        header.set_size(size);
        header.set_cksum();

        if let Err(e) = tar.append(&header, Cursor::new(buf)) {
            die(format_args!("failure appending \"{}\" to tar: {}", file.name(), e));
        }
    }

    if let Err(e) = tar.into_inner() {
        die(format_args!("error finishing tar file: {}", e));
    }
}

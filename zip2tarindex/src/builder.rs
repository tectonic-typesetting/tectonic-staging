use std::io::{self, SeekFrom};
use std::io::prelude::*;

use Header;

/// A structure for building archives
///
/// This structure has methods for building up an archive from scratch into any
/// arbitrary writer.
pub struct HackedBuilder<WO: Write + Seek, WI: Write> {
    finished: bool,
    obj: Option<WO>,
    index: WI,
}

impl<WO: Write + Seek, WI: Write> HackedBuilder<WO, WI> {
    /// Create a new archive builder with the underlying object as the
    /// destination of all data written.
    pub fn new(obj: WO, index: WI) -> HackedBuilder<WO, WI> {
        HackedBuilder {
            finished: false,
            obj: Some(obj),
            index: index,
        }
    }

    fn inner(&mut self) -> &mut WO {
        self.obj.as_mut().unwrap()
    }

    /// Unwrap this archive, returning the underlying object.
    ///
    /// This function will finish writing the archive if the `finish` function
    /// hasn't yet been called, returning any I/O error which happens during
    /// that operation.
    pub fn into_inner(mut self) -> io::Result<WO> {
        if !self.finished {
            try!(self.finish());
        }
        Ok(self.obj.take().unwrap())
    }
   
    /// Adds a new entry to this archive.
    ///
    /// This function will append the header specified, followed by contents of
    /// the stream specified by `data`. To produce a valid archive the `size`
    /// field of `header` must be the same as the length of the stream that's
    /// being written. Additionally the checksum for the header should have been
    /// set via the `set_cksum` method.
    ///
    /// Note that this will not attempt to seek the archive to a valid position,
    /// so if the archive is in the middle of a read or some other similar
    /// operation then this may corrupt the archive.
    ///
    /// Also note that after all entries have been written to an archive the
    /// `finish` function needs to be called to finish writing the archive.
    ///
    /// # Errors
    ///
    /// This function will return an error for any intermittent I/O error which
    /// occurs when either reading or writing.
    ///
    /// # Examples
    ///
    /// ```
    /// use tar::{HackedBuilder, Header};
    ///
    /// let mut header = Header::new_gnu();
    /// header.set_path("foo");
    /// header.set_size(4);
    /// header.set_cksum();
    ///
    /// let mut data: &[u8] = &[1, 2, 3, 4];
    ///
    /// let mut ar = HackedBuilder::new(Vec::new());
    /// ar.append(&header, data).unwrap();
    /// let data = ar.into_inner().unwrap();
    /// ```
    pub fn append<R: Read>(&mut self, header: &Header, mut data: R)
                           -> io::Result<()> {
        // tar-rs implements this method by handing off to a standalone
        // function, but its style doesn't work for us since we can't get away
        // with just trait objects. (It's not clear to me why tar-rs does
        // things the way it does.) We just copy the implementation here.
        let mut dst = self.obj.as_mut().unwrap();

        try!(dst.write_all(header.as_bytes()));
        let pos = dst.seek(SeekFrom::Current(0)).unwrap();

        let len = try!(io::copy(&mut data, &mut dst));

        // THE HACK: write out an index line.
        writeln!(self.index, "{} {} {}", header.path().unwrap().display(),
                 pos, len).expect("index write failed");
        
        // Pad with zeros if necessary.
        let buf = [0; 512];
        let remaining = 512 - (len % 512);
        if remaining < 512 {
            try!(dst.write_all(&buf[..remaining as usize]));
        }

        Ok(())
    }

    /// Finish writing this archive, emitting the termination sections.
    ///
    /// This function should only be called when the archive has been written
    /// entirely and if an I/O error happens the underlying object still needs
    /// to be acquired.
    ///
    /// In most situations the `into_inner` method should be preferred.
    pub fn finish(&mut self) -> io::Result<()> {
        if self.finished {
            return Ok(())
        }
        self.finished = true;
        self.inner().write_all(&[0; 1024])
    }
}

impl<WO: Write + Seek, WI: Write> Drop for HackedBuilder<WO, WI> {
    fn drop(&mut self) {
        let _ = self.finish();
    }
}

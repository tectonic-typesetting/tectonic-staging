#! /bin/bash
# Copyright 2016 the Tectonic Project.
# Licensed under the MIT License.

image_name=tectonic-texlive-builder
builder_cont_name=tectonic-bld-cont
state_dir=$(pwd)/state # symlink here!

set -e

if [ -z "$1" -o "$1" = help ] ; then
    echo "You must supply a subcommand. Subcommands are:

bintray-upload    -- Upload an indexed tar file to Bintray.
build-image       -- Create/update to builder Docker image.
builder-bash      -- Run a shell in a temporary builder container.
ingest-source     -- Copy needed source code from TeXLive to this repo.
init-build        -- Initialize a Docker-based compilation of the TeXLive binaries.
make-installation -- Install TeXLive into a new directory tree.
make-base-zipfile -- Make a Zip file of a standardized base TeXLive installation.
svn-pull          -- Update the Git mirror of the TeXLive SVN repository.
update-containers -- Rebuild the TeXLive \"container\" files.
update-products   -- Update the prettified C code in \"products/\".
zip2itar          -- Convert a bundle from Zip format to indexed-tar format.

"
    exit 1
fi

command="$1"
shift


function die () {
    echo >&2 "error:" "$@"
    exit 1
}


function bintray_upload () {
    tar_path="$1"
    version="$2"
    package_id="tl2016extras"

    [ x"$2" != x ] || die "usage: staging.sh bintray-upload <tar-path> <version>"
    [ x"$BINTRAY_API_KEY" != x ] || die "you must set the \$BINTRAY_API_KEY environment variable"

    curl -T $tar_path.index.gz -u"pkgw:$BINTRAY_API_KEY" \
         -H "X-Bintray-Package:$package_id" -H "X-Bintray-Version:$version" \
         https://api.bintray.com/content/pkgw/tectonic/$package_id/$version/$(basename $tar_path).index.gz

    curl -T $tar_path -u"pkgw:$BINTRAY_API_KEY" \
         -H "X-Bintray-Package:$package_id" -H "X-Bintray-Version:$version" \
         https://api.bintray.com/content/pkgw/tectonic/$package_id/$version/$(basename $tar_path)
}


function build_image () {
    tag=$(date +%Y%m%d)
    docker build -t $image_name:$tag builder/
    docker tag $image_name:$tag $image_name:latest
}


function builder_bash () {
    [ -d $state_dir/repo ] || die "no such directory $state_dir/repo"
    exec docker run -it --rm -v $state_dir:/state:rw,z $image_name bash
}


function ingest_source () {
    [ -d $state_dir/rbuild ] || die "no such directory $state_dir/rbuild"
    exec ./ingest-source.sh
}


function init_build() {
    [ -d $state_dir/repo ] || die "no such directory $state_dir/repo"
    [ ! -d $state_dir/rbuild ] || die "directory $state_dir/rbuild may not exist before starting build"
    docker create \
           -v $state_dir:/state:rw,z \
           -i -t \
           --name $builder_cont_name \
           $image_name bash || die "could not create builder container $builder_cont_name"
    docker start $builder_cont_name || die "could not start builder container $builder_cont_name"
    exec docker exec $builder_cont_name /entrypoint.sh init-build
}


function make_installation () {
    # arguments: names of TeXLive packages to install above and beyond the
    # "minimal" installation profile.

    [ -d $state_dir/repo ] || die "no such directory $state_dir/repo"

    dest=$(mktemp -d -p $state_dir install.XXXXXX)
    destbase=$(basename $dest)
    cdest=/state/$destbase

    # $cdest/texmf-dist is created and populated with most files.
    # TEXMFSYSCONFIG and TEXMFSYSVAR are filled with files that we might care about.
    # TEXMFLOCAL is created but doesn't have anything we care about.
    # TEXMFHOME, TEXMFCONFIG, and TEXMFVAR are not created.
    # option_sys_* are not created either.
    # Other settings are best guesses about what's sensible.

    cat >$dest/builder.profile <<-EOF
    selected_scheme scheme-minimal
    TEXDIR $cdest
    TEXMFSYSCONFIG $cdest/texmf-dist
    TEXMFSYSVAR $cdest/texmf-dist
    TEXMFLOCAL $cdest/texmf-local
    TEXMFHOME $cdest/texmf-home
    TEXMFCONFIG $cdest/texmf-config
    TEXMFVAR $cdest/texmf-var
    collection-basic 1
    in_place 0
    option_adjustrepo 1
    option_autobackup 0
    option_desktop_integration 0
    option_doc 0
    option_file_assocs 0
    option_fmt 1
    option_letter 1
    option_menu_integration 0
    option_path 0
    option_post_code 1
    option_src 0
    option_sys_bin $cdest/sys-bin
    option_sys_info $cdest/sys-info
    option_sys_man $cdest/sys-man
    option_w32_multi_user 0
    option_write18_restricted 0
    portable 0
EOF
    echo $dest
    set +e
    docker run --rm -v $state_dir:/state:rw,z $image_name \
	   install-profile $cdest/builder.profile $cdest $(id -u):$(id -g) "$@" &>$dest/outer.log
    ec=$?
    [ $ec -eq 0 ] || die "install-tl failed; see $dest/outer.log"
    set -e
}


function make_base_zipfile () {
    zip="$1"
    bundle_id=tlextras2016
    shift

    # First, TeXLive package installation.

    installdir=$(make_installation \
         collection-basic \
         collection-fontsextra \
         collection-fontsrecommended \
         collection-genericextra \
         collection-genericrecommended \
         collection-latexextra \
         collection-latexrecommended \
         collection-latex \
         collection-luatex \
         collection-mathextra \
         collection-plainextra \
         collection-publishers \
         collection-science \
         collection-xetex \
         collection-langafrican \
         collection-langarabic \
         collection-langchinese \
         collection-langcjk \
         collection-langcyrillic \
         collection-langczechslovak \
         collection-langenglish \
         collection-langeuropean \
         collection-langfrench \
         collection-langgerman \
         collection-langgreek \
         collection-langindic \
         collection-langitalian \
         collection-langjapanese \
         collection-langkorean \
         collection-langother \
         collection-langpolish \
         collection-langportuguese \
         collection-langspanish
    )

    # Some manual fiddles for format file generation

    cp extras/$bundle_id/* $installdir/texmf-dist/

    # Finally, turn it all into a Zip file.

    ./builder/make-zipfile.py "$installdir" "$zip"
    rm -rf "$installdir"
}


function svn_pull () {
    [ -d $state_dir/repo ] || die "no such directory $state_dir/repo"
    cd $state_dir/repo
    git svn fetch
    exec git merge --ff-only svn/heads/trunk
}


function update_containers () {
    [ -d $state_dir/repo ] || die "no such directory $state_dir/repo"
    mkdir -p $state_dir/containers $state_dir/versioned
    docker run --rm -v $state_dir:/state:rw,z $image_name update-containers

    # Make versioned copies of unmodified packages.

    cd "$state_dir"

    (cd containers/archive && ls -1) |while read cname ; do
	keep=false
	# TBD: are we going to need versioned packages of the binaries?
	case $cname in
	    *.doc.tar.xz | *.source.tar.xz | *.*-*.tar.xz | *.win32.tar.xz) ;;
	    *.tar.xz) keep=true ;;
	esac
	$keep || continue

	pkg=$(basename $cname .tar.xz)
	new=containers/archive/$cname
	tlp=tlpkg/tlpobj/$pkg.tlpobj
	rev=$(tar xf $new -O $tlp |grep ^revision |awk '{print $2}')
	versioned=versioned/$pkg-$rev.tar.xz

	if [ ! -f $versioned ] ; then
	    echo $pkg $rev
	    cp $new $versioned
	    chmod 444 $versioned
	fi
    done
}


function update_products () {
    [ -d $state_dir/sbuild ] || die "no such directory $state_dir/sbuild"
    for f in xetex-pool.c xetexini.c xetex0.c xetexcoerce.h xetexd.h bibtex.c bibtex.h ; do
	cp $state_dir/sbuild/$f products/
	indent -linux -nut -i4 -l120 products/$f
	rm -f products/${f}~
    done
}


function zip2itar () {
    zipfile="$1"

    dir=$(cd $(dirname "$zipfile") && pwd)
    zipfull=$dir/$(basename "$zipfile")
    tarfull=$dir/$(basename "$zipfile" .zip).tar
    echo "Generating $tarfull ..."
    cd $(dirname $0)/zip2tarindex
    exec cargo run --release -- "$zipfull" "$tarfull"
}


# Dispatch subcommands.

case "$command" in
    bintray-upload)
	bintray_upload "$@" ;;
    build-image)
	build_image "$@" ;;
    builder-bash)
	builder_bash "$@" ;;
    ingest-source)
	ingest_source "$@" ;;
    init-build)
	init_build "$@" ;;
    make-installation)
	make_installation "$@" ;;
    make-base-zipfile)
	make_base_zipfile "$@" ;;
    svn-pull)
	svn_pull "$@" ;;
    update-containers)
	update_containers "$@" ;;
    update-products)
	update_products "$@" ;;
    zip2itar)
        zip2itar "$@" ;;
    *)
	echo >&2 "error: unrecognized command \"$command\"."
	exit 1 ;;
esac

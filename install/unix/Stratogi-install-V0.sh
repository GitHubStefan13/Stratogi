#!/bin/bash

# Display usage
cpack_usage()
{
  cat <<EOF
Usage: $0 [options]
Options: [defaults in brackets after descriptions]
  --help            print this message
  --prefix=dir      directory in which to install
  --include-subdir  include the install-VX subdirectory
  --exclude-subdir  exclude the install-VX subdirectory
EOF
  exit 1
}

cpack_echo_exit()
{
  echo $1
  exit 1
}

# Display version
cpack_version()
{
  echo "Stratogi Installer Version: 1.0.0, Copyright 2020 (c) Stefan Unruh"
}

# Helper function to fix windows paths.
cpack_fix_slashes ()
{
  echo "$1" | sed 's/\\/\//g'
}
package_install_prefix_directory=~/PiControlServer
package_install_dir="."
package_project_name="PiControlServer"
package_executable_name="PiControlServer"
user_name="`id -un`"
interactive=TRUE
cpack_skip_license=TRUE
cpack_include_subdir=""
for a in "$@"; do
  if echo $a | grep "^--prefix=" > /dev/null 2> /dev/null; then
    cpack_prefix_dir=`echo $a | sed "s/^--prefix=//"`
    cpack_prefix_dir=`cpack_fix_slashes "${cpack_prefix_dir}"`
  fi
  if echo $a | grep "^--help" > /dev/null 2> /dev/null; then
    cpack_usage 
  fi
  if echo $a | grep "^--version" > /dev/null 2> /dev/null; then
    cpack_version 
    exit 2
  fi
  if echo $a | grep "^--include-subdir" > /dev/null 2> /dev/null; then
    cpack_include_subdir=TRUE
  fi
  if echo $a | grep "^--exclude-subdir" > /dev/null 2> /dev/null; then
    cpack_include_subdir=FALSE
  fi
  if echo $a | grep "^--skip-license" > /dev/null 2> /dev/null; then
    cpack_skip_license=TRUE
  fi
done

echo "Prefix ${cpack_prefix_dir} entered..."
if [ "x${cpack_include_subdir}x" != "xx" -o "x${cpack_skip_license}x" = "xTRUEx" ]
then
  interactive=FALSE
fi

cpack_version
echo "Hello ${user_name}, this is a self-extracting archive."


if [ "x${cpack_prefix_dir}x" != "xx" ]
then
  toplevel="${cpack_prefix_dir}"
fi

if [ "x${toplevel}x" = "xx" ]
then
  toplevel="${package_install_prefix_directory}"
fi

if [ "x${toplevel}x" = "xx" ]
then
  toplevel="`pwd`"
fi

echo "The archive will be extracted to: ${toplevel}"

if [ "x${interactive}x" = "xTRUEx" ]
then
  echo ""
  echo "If you want to stop extracting, please press <ctrl-C>."

  if [ "x${cpack_skip_license}x" != "xTRUEx" ]
  then
    more << '____cpack__here_doc____'
LICENSE
=======

This is an installer created using CPack (https://cmake.org). No license provided.


____cpack__here_doc____
    echo
    echo "Do you accept the license? [yN]: "
    read line leftover
    case ${line} in
      y* | Y*)
        cpack_license_accepted=TRUE;;
      *)
        echo "License not accepted. Exiting ..."
        exit 1;;
    esac
  fi
fi

mkdir -p "${toplevel}"

echo
echo "Using target directory: ${toplevel}"
echo "Extracting, please wait..."
echo ""

# take the archive portion of this file and pipe it to tar
# the NUMERIC parameter in this command should be one more
# than the number of lines in this header file
# there are tails which don't understand the "-n" argument, e.g. on SunOS
# OTOH there are tails which complain when not using the "-n" argument (e.g. GNU)
# so at first try to tail some file to see if tail fails if used with "-n"
# if so, don't use "-n"
use_new_tail_syntax="-n"
tail $use_new_tail_syntax +1 "$0" > /dev/null 2> /dev/null || use_new_tail_syntax=""

tail $use_new_tail_syntax +171 "$0" | gunzip | (cd "${toplevel}" && tar xf -) || cpack_echo_exit "Problem unpacking the installer"

echo "Unpacking to ${toplevel} finished successfully!"

mkdir -p ${HOME}/.local/share/applications
cp ${toplevel}/${package_executable_name}.desktop ${HOME}/.local/share/applications/${package_executable_name}.desktop

if [ -d "${HOME}/Desktop" ]
then
  echo "Copy ${package_executable_name}.desktop to ${HOME}/Desktop/${package_executable_name}.desktop"
  cp ${toplevel}/${package_executable_name}.desktop ~/Desktop/${package_executable_name}.desktop
fi
if [ -d "${HOME}/Arbeitsfläche" ]
then
  echo "Copy ${package_executable_name}.desktop to ${HOME}/Arbeitsfläche/${package_executable_name}.desktop"
  cp ${toplevel}/${package_executable_name}.desktop ~/Arbeitsfläche/${package_executable_name}.desktop
fi
if [ -d "${HOME}/Arbeitsfl che" ]
then
  echo "Copy ${package_executable_name}.desktop to ${HOME}/Arbeitsfl che/${package_executable_name}.desktop"
  cp ${toplevel}/${package_executable_name}.desktop ~/Arbeitsfl che/${package_executable_name}.desktop
fi
if [ -d "${HOME}/Schreibtisch" ]
then
  echo "Copy ${package_executable_name}.desktop to ${HOME}/Schreibtisch/${package_executable_name}.desktop"
  cp ${toplevel}/${package_executable_name}.desktop ~/Schreibtisch/${package_executable_name}.desktop
fi
echo ""
bash ${toplevel}/${package_executable_name}.sh > /dev/null 2>&1 &

exit 0
#-----------------------------------------------------------
#      Start of TAR.GZ file
#-----------------------------------------------------------;

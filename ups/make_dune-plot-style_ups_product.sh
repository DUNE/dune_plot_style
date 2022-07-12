#!/bin/bash

# heavily inspired by NOvAs ups product making script
# 2022-07-12

# print usage
usage() {

  echo ""
  echo "Usage is:" 
  echo "   sh make_dune-plot-style_ups_product.sh <version_number>"
  echo ""
  echo "The version number should be of the form vXX_XX."
  echo ""
  exit -1

}

# setup
# -----------------------------------------------------------------------------

if [ ${#@} != 1 ]; then
  usage
fi 

# useful vars
prodpath=/grid/fermiapp/products/dune/
prodname="dune-plot-style" 
version=$1
tmpdir=/tmp/${prodname}_${version}

if [[ ! ${version} =~ v[0-9][0-9]_[0-9][0-9] ]]; then
  echo ""
  echo "Version pattern not allowed."
  usage
fi

echo "Tagging ${prodname} ${version}"

source ${prodpath}/setup_dune.sh
setup upd

echo "Printing active products"
echo "-------------------------------------------"
ups active
echo "-------------------------------------------"

# check if the first argument is a valid path
if [  -d "${tmpdir}" ]; then
  echo "${tmpdir} is already exists! Cannot do clean checkout!"
  echo "Aborting"
  exit -1
fi

# pull the latest version of the git repo and put it in the product directory
# -----------------------------------------------------------------------------

# clone to temp directory
git clone git@github.com:DUNE/${prodname}.git ${tmpdir}/${prodname}

proddir=${prodpath}/${prodname}
dest=${proddir}/${version}/NULL

echo "$prodname will be created in $dest"

if [ ! -d "${proddir}" ]; then
  mkdir -p ${proddir}
fi

# offer option to overwrite a product if it exists, though
# in general we won't want to do this
if [ -d "${proddir}/${version}" ]; then
  echo ""
  echo "Product ${prodname} with version ${version} already exists." 
  echo "Making it again will over-write the existing one."
  echo ""
  read -p "Are you sure you want to proceed (y/n)? " -n 1 -r
  echo   
  if [[ $REPLY =~ ^[Yy]$ ]]
  then
    echo "OK. If you say so."
  else
    echo "The script will now abort. Try again with a different version."
    exit -1
  fi
fi

# now copy the code to it's location in the /grid area
mkdir -p ${dest}
rsync --exclude '*~' --exclude '*.git' -rL $tmpdir/${reponame}/${prodname}/* ${dest}

# update the ups table to give the correct version number
ups_table=${dest}/ups/${prodname}.table
if [ ! -f "${ups_table}" ] ; then
  echo ""
  echo "Error! UPS table ${ups_table} does not exist!"
  echo ""
  exit -1
fi

echo "Updating table file"

sed -i -e "s:XXVERSIONXX:${version}:" \
  ${ups_table}

echo"Declaring product ${prodname} with version ${version} to UPS."

# declare to ups
ups declare -f NULL -z ${prodpath} \
  -r ${prodpath}/${prodname}/${version}/NULL \
  -m ${prodname}.table \
  ${prodname} ${version}

retval=$?
test $retval -ne 0 && echo "Error! 'ups declare' returned non-zero - BAILING" && exit 1

# add to upd
cd ${proddir}/${version}/NULL/

upd addproduct ${prodname} ${version} 
retval=$?
test $retval -ne 0 && echo "Error! 'upd addproduct' returned non-zero - BAILING" && exit 1

rm -rf ${tmpdir}

echo "Done"

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
path=/grid/fermiapp/products/dune/
reponame="dune-plot-style" 
version=$1
tmpdir=/tmp/${reponame}_${version}

if [[ ! ${version} =~ v[0-9][0-9]_[0-9][0-9] ]]; then
  echo ""
  echo "Version pattern not allowed."
  usage
fi

echo "Tagging ${reponame} ${version}"

source ${path}/setup_dune.sh
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
git clone -b ${version} git@github.com:DUNE/${reponame}.git ${tmpdir}/${reponame}-preorg

# need to do some reogranising for the ups product
mkdir -p ${tmpdir}/${reponame}/python/dunestyle/matplotlib
mkdir -p ${tmpdir}/${reponame}/python/dunestyle/root
mv ${tmpdir}/${reponame}-preorg/src/matplotlib/stylelib  ${tmpdir}/${reponame}/
mv ${tmpdir}/${reponame}-preorg/src/root/cpp/include     ${tmpdir}/${reponame}/
mv ${tmpdir}/${reponame}-preorg/src/__init__.py          ${tmpdir}/${reponame}/python/dunestyle/
mv ${tmpdir}/${reponame}-preorg/src/matplotlib/python/*  ${tmpdir}/${reponame}/python/dunestyle/matplotlib
mv ${tmpdir}/${reponame}-preorg/src/root/python/*        ${tmpdir}/${reponame}/python/dunestyle/root
rm -rf ${tmpdir}/${reponame}-preorg

proddir=${path}/${reponame}
dest=${proddir}/${version}/NULL

echo "$reponame will be created in $dest"

if [ ! -d "${proddir}" ]; then
  mkdir -p ${proddir}
fi

# offer option to overwrite a product if it exists, though
# in general we won't want to do this
if [ -d "${proddir}/${version}" ]; then
  echo ""
  echo "Product ${reponame} with version ${version} already exists." 
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
rsync --exclude '*~' --exclude '*.git' -rL $tmpdir/${reponame}/* ${dest}

# update the ups table to give the correct version number
ups_table=${dest}/ups/${reponame}.table
if [ ! -f "${ups_table}" ] ; then
  echo ""
  echo "Error! UPS table ${ups_table} does not exist!"
  echo ""
  exit -1
fi

echo "Updating table file"

sed -i -e "s:XXVERSIONXX:${version}:" \
  ${ups_table}

echo"Declaring product ${reponame} with version ${version} to UPS."

# declare to ups
ups declare -f NULL -z ${path} \
  -r ${path}/${reponame}/${version}/NULL \
  -m ${reponame}.table \
  ${reponame} ${version}

retval=$?
test $retval -ne 0 && echo "Error! 'ups declare' returned non-zero - BAILING" && exit 1

# add to upd
cd ${proddir}/${version}/NULL/

upd addproduct ${reponame} ${version} 
retval=$?
test $retval -ne 0 && echo "Error! 'upd addproduct' returned non-zero - BAILING" && exit 1

rm -rf ${tmpdir}

echo "Done"

#!/bin/bash
# gwn_modtool : creates new block an converts code from block template


### verify syntax and number of arguments, exec from build directory
CURDIR=`pwd`
GREPBUILD=`echo $CURDIR | grep /build$`

if [ $# -ne 4 ]           # verify number of arguments
then
  echo "gwn_modtool syntax:"
  echo "  gwn_modtool block_name nr_inputs nr_outputs nr_timers"
  echo "Must be executed from the build directory"
  echo
  exit
elif [ -z $GREPBUILD ]    # verify execution from ./build directory"
then
  echo "gwn_modtool:"
  echo "  must be executed from the build directory"
  exit
fi

MODULE_NAME=`ls -1 ../include/ | cut -f2 -d"/"`
echo "Module Name: "${MODULE_NAME}

### block properties: name, ports, timers

# block properties from positional arguments
BLOCK_NAME=$1
NR_IN=$2
NR_OUT=$3
NR_TIMERS=$4

# assign block block properties to object attributes
SED_BLK_NAME="s/d_name = p_name/dname = $BLOCK_NAME/g"
SED_NR_IN="s/d_number_in = $NR_IN/g"
SED_NR_OUT="s/d_number_out = $NR_OUT/g"
SED_TIMERS="s/d_number_timers = $NR_TIMERS/g"


### produce user arguments declaration and initialization

# strings to substitute
MSG_USER_ARGS="<GWN user arguments list>"
MSG_USER_ARGS_DECL="GWN user arguments declaration"
MSG_USER_ARGS_INIT="GWN user arguments initialization"
MSG_USER_PARS="<GWN user parameters list>"

# user parameters: signature, declaration, initialization
USER_ARGS=""         # user arguments, signature of function
USER_ARGS_DECL=""    # user arguments declaration
USER_ARGS_INIT=""    # user arguments initialization
USER_PARS=""         # user parameters, for function call

# user arguments list, create array of arguments
echo -n "Enter argument list: "; read USER_ARGS
#USER_ARGS="std::string message, int counter, std::string port"  # for testing
USER_ARGS_ARRAY=(`echo $USER_ARGS | tr " " "#" | tr "," " "`)


for WD in ${USER_ARGS_ARRAY[*]}
do
  LS_ITEM_ARGS=`echo $WD | tr "#" " "`
  VAR=(${LS_ITEM_ARGS})

  VAR_NR_LAST=$((${#VAR[@]} - 1))    # index of last argument
  #echo "Last item in VAR" $VAR_NR_LAST
  #echo "Var declaration": $WD
  #echo -n "Items in declaraton:"

  #echo; echo "VARIABLES y última"
  ITEM_ARG_DECL=""
  ITEM_ARG_INIT=""
  ITEM_PAR=""
  for I in "${!VAR[@]}"
  do
    if [ $I -eq $VAR_NR_LAST ]
    then
      ITEM_ARG_DECL=$ITEM_ARG_DECL"d_"${VAR[$I]}";"
      ITEM_ARG_INIT=$ITEM_ARG_INIT"d_"${VAR[$I]}" = "${VAR[$I]}";"
      ITEM_PAR=${ITEM_PAR}${VAR[$I]}", "
    else
      ITEM_ARG_DECL=$ITEM_ARG_DECL${VAR[$I]}" "
    fi
  done
  #echo $ITEM_ARG_DECL --- $ITEM_ARG_INIT

  USER_ARGS_DECL=$USER_ARGS_DECL"\n      "$ITEM_ARG_DECL
  USER_ARGS_INIT=$USER_ARGS_INIT"\n      "$ITEM_ARG_INIT
  USER_PARS=${USER_PARS}${ITEM_PAR}
done
USER_PARS=`echo ${USER_PARS} | sed -E "s/,\$//g"`  # delete last ","

echo User argument list: "$USER_ARGS"
echo User parameter list: "$USER_PARS"
echo -e User arguments declaration: "$USER_ARGS_DECL"
echo -e User arguments initialization: "$USER_ARGS_INIT"


### create block with gr_modtool

echo -n "About to create block $1; proceed(YyNn)? "; read ANSWER
#ANSWER="N"
if [ "$ANSWER" == "y" -o "$ANSWER" == "Y" ]
then
  echo "Creating block $1..."
else
  echo "Answer was not y nor Y:" $ANSWER
#  exit
fi

# sed substitution expressions
BLOCK_NAME_CAPS=`echo $BLOCK_NAME | tr [a-z] [A-Z]`
SED_GWNBLOCKC="s/gwnblockc/${BLOCK_NAME}/g"
SED_GWNBLOCKC_CAPS="s/GWNBLOCKC/${BLOCK_NAME_CAPS}/g"
#SED_PARAMETERS=\"s/$DEFAULT_PARS/$PARAMETERS/g\"

# create files for new blocks
cd ..   # from build directory to module root directory
echo -en "... to module root directory to execute gr_modtool:\n      "; pwd
GRMODTOOL=`which gr_modtool`
echo ... gr_modtool, creating $BLOCK_NAME
python $GRMODTOOL add --block-name=$BLOCK_NAME \
  --block-type=general --lang=cpp \
  --argument-list="" --add-python-qa
echo ...gr_modtool, creating ${BLOCK_NAME}_pdata
python $GRMODTOOL add --block-name=${BLOCK_NAME}_pdata --block-type=noblock --lang=cpp --argument-list="" 
cd $CURDIR
echo -en "... returned to module build directory:\n      "; pwd


### transform files by substitution in template files

# new block files
echo "... processing ../include/${MODULE_NAME}/${BLOCK_NAME}.h"
sed -e $SED_GWNBLOCKC -e $SED_GWNBLOCKC_CAPS \
  -e "s/$MSG_USER_ARGS/$USER_ARGS/g" \
  ../libgwn/gwnblockc.h > ../include/${MODULE_NAME}/${BLOCK_NAME}.h

echo "... processing ../lib/${BLOCK_NAME}_impl.h"
sed -e $SED_GWNBLOCKC -e $SED_GWNBLOCKC_CAPS \
  -e "s/${MSG_USER_ARGS}/$USER_ARGS/g" \
  -e "s/${MSG_USER_ARGS_DECL}/&${USER_ARGS_DECL}/g" \
  ../libgwn/gwnblockc_impl.h > ../lib/${BLOCK_NAME}_impl.h

echo "... processing ../lib/${BLOCK_NAME}_impl.cc"
sed -e $SED_GWNBLOCKC -e $SED_GWNBLOCKC_CAPS \
  -e "s/${MSG_USER_ARGS}/${USER_ARGS}/g" \
  -e "s/${MSG_USER_PARS}/${USER_PARS}/g" \
  -e "s/${MSG_USER_ARGS_INIT}/&${USER_ARGS_INIT}/g" \
  -e "s/d_name = \"no_name\"/d_name = \"$BLOCK_NAME\"/g" \
  -e "s/d_number_in = 0/d_number_in = $NR_IN/g" \
  -e "s/d_number_out = 0/d_number_out = $NR_OUT/g" \
  -e "s/d_number_timers = 0/d_number_timers = $NR_TIMERS/g" \
  ../libgwn/gwnblockc_impl.cc > ../lib/${BLOCK_NAME}_impl.cc

# new block QA file
echo "... processing ../python/qa_${BLOCK_NAME}.py"
sed -e $SED_GWNBLOCKC \
  ../libgwn/qa_gwnblockc.py > ../python/qa_${BLOCK_NAME}.py

# new block process data files
echo "... processing ../include/${MODULE_NAME}/${BLOCK_NAME}_pdata.h"
sed -e $SED_GWNBLOCKC -e $SED_GWNBLOCKC_CAPS \
  -e "s/${MSG_USER_ARGS}/$USER_ARGS/g" \
  -e "s/${MSG_USER_ARGS_DECL}/&${USER_ARGS_DECL}/g" \
  ../libgwn/gwnblockc_pdata.h > ../include/${MODULE_NAME}/${BLOCK_NAME}_pdata.h

echo "... processing ../lib/${BLOCK_NAME}_pdata.cc"
sed -e $SED_GWNBLOCKC -e $SED_GWNBLOCKC_CAPS \
  -e "s/${MSG_USER_ARGS}/${USER_ARGS}/g" \
  -e "s/${MSG_USER_PARS}/${USER_PARS}/g" \
  -e "s/${MSG_USER_ARGS_INIT}/&${USER_ARGS_INIT}/g" \
  ../libgwn/gwnblockc_pdata.cc > ../lib/${BLOCK_NAME}_pdata.cc

echo "... block $1 created."


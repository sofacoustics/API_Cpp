#!/bin/sh

NODEJS=`which node || which nodejs || which false`

# make json
../build/src/sofa2json "$1".sofa >tmp1.json 2>tmp1.txt 

ret=$?
if [ "$ret" != 0 ]; then 
    cat tmp1.txt
    echo Error libsofa sofa2json $ret
    exit $ret
fi

# make sofa
../build/src/json2sofa tmp1.json tmp2.sofa 2>tmp2.txt 

ret=$?
if [ "$ret" != 0 ]; then 
    cat tmp2.txt
    echo Error libsofa json2sofa $ret
    exit $ret
fi

# make json
../build/src/sofa2json tmp2.sofa >tmp3.json 2>tmp3.txt 

ret=$?
if [ "$ret" != 0 ]; then 
    cat tmp3.txt
    echo Error libsofa 2nd sofa2json $ret
    exit $ret
fi

# compare
cp -f ../tests/json-diff.js . 2>/dev/null || true
$NODEJS ./json-diff.js ./tmp1.json ./tmp3.json
ret=$?
if [ "$ret" != 0 ]; then 
    echo Diff $ret
    exit $ret
fi
echo ok
#rm tmp1.json tmp1.txt tmp2.sofa tmp2.txt tmp3.json tmp3.txt json-diff.js


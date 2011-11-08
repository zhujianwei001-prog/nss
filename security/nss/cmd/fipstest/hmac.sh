#!/bin/sh
#
# A Bourne shell script for running the NIST HMAC Algorithm Validation Suite
#
# Before you run the script, set your PATH, LD_LIBRARY_PATH, ... environment
# variables appropriately so that the fipstest command and the NSPR and NSS
# shared libraries/DLLs are on the search path.  Then run this script in the
# directory where the REQUEST (.req) files reside.  The script generates the
# RESPONSE (.rsp) files in the same directory.

BASEDIR=${1-.}
TESTDIR=${BASEDIR}/HMAC
COMMAND=${2-run}
REQDIR=${BASEDIR}/req
RSPDIR=${BASEDIR}/resp
                               
hmac_requests="
HMAC.req
"

if [ ${COMMAND} = "verify" ]; then
    for request in $hmac_requests; do
	sh ./validate1.sh ${TESTDIR} $request
    done
    exit 0
fi
for request in $hmac_requests; do
    response=`echo $request | sed -e "s/req/rsp/"`
    echo $request $response
    fipstest hmac ${REQDIR}/$request > ${RSPDIR}/$response
done


/* 
 * The contents of this file are subject to the Mozilla Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 * 
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 * 
 * The Original Code is the Netscape security libraries.
 * 
 * The Initial Developer of the Original Code is Netscape
 * Communications Corporation.  Portions created by Netscape are 
 * Copyright (C) 1994-2000 Netscape Communications Corporation.  All
 * Rights Reserved.
 * 
 * Contributor(s):
 * 
 * Alternatively, the contents of this file may be used under the
 * terms of the GNU General Public License Version 2 or later (the
 * "GPL"), in which case the provisions of the GPL are applicable 
 * instead of those above.  If you wish to allow use of your 
 * version of this file only under the terms of the GPL and not to
 * allow others to use your version of this file under the MPL,
 * indicate your decision by deleting the provisions above and
 * replace them with the notice and other provisions required by
 * the GPL.  If you do not delete the provisions above, a recipient
 * may use your version of this file under either the MPL or the
 * GPL.
 */

#ifndef NSSBASE_H
#define NSSBASE_H

#ifdef DEBUG
static const char NSSBASE_CVS_ID[] = "@(#) $RCSfile$ $Revision$ $Date$ $Name$";
#endif /* DEBUG */

/*
 * nssbase.h
 *
 * This header file contains the prototypes of the basic public
 * NSS routines.
 */

#ifndef NSSBASET_H
#include "nssbaset.h"
#endif /* NSSBASET_H */

#ifndef NSSERRORS_H
#include "nsserrors.h"
#endif /* NSSERRORS_H */

PR_BEGIN_EXTERN_C

/*
 * NSSArena
 *
 * The public methods relating to this type are:
 *
 *  NSSArena_Create  -- constructor
 *  NSSArena_Destroy
 */

/*
 * NSSArena_Create
 *
 * This routine creates a new memory arena.  This routine may return
 * NULL upon error, in which case it will have created an error stack.
 *
 * The top-level error may be one of the following values:
 *  NSS_ERROR_NO_MEMORY
 *
 * Return value:
 *  NULL upon error
 *  A pointer to an NSSArena upon success
 */

NSS_EXTERN NSSArena *
NSSArena_Create (
  void
);

/*
 * NSSArena_Destroy
 *
 * This routine will destroy the specified arena, freeing all memory
 * allocated from it.  This routine returns a PRStatus value; if 
 * successful, it will return PR_SUCCESS.  If unsuccessful, it will
 * create an error stack and return PR_FAILURE.
 *
 * The top-level error may be one of the following values:
 *  NSS_ERROR_INVALID_ARENA
 *
 * Return value:
 *  PR_SUCCESS upon success
 *  PR_FAILURE upon failure
 */

NSS_EXTERN PRStatus
NSSArena_Destroy (
  NSSArena *arena
);

/*
 * The error stack
 *
 * The public methods relating to the error stack are:
 *
 *  NSS_GetError
 *  NSS_GetErrorStack
 */

/*
 * NSS_GetError
 *
 * This routine returns the highest-level (most general) error set
 * by the most recent NSS library routine called by the same thread
 * calling this routine.
 *
 * This routine cannot fail.  It may return NSS_ERROR_NO_ERROR, which
 * indicates that the previous NSS library call did not set an error.
 *
 * Return value:
 *  0 if no error has been set
 *  A nonzero error number
 */

NSS_EXTERN NSSError
NSS_GetError (
  void
);

/*
 * NSS_GetErrorStack
 *
 * This routine returns a pointer to an array of NSSError values, 
 * containingthe entire sequence or "stack" of errors set by the most 
 * recent NSS library routine called by the same thread calling this 
 * routine.  NOTE: the caller DOES NOT OWN the memory pointed to by 
 * the return value.  The pointer will remain valid until the calling 
 * thread calls another NSS routine.  The lowest-level (most specific) 
 * error is first in the array, and the highest-level is last.  The 
 * array is zero-terminated.  This routine may return NULL upon error; 
 * this indicates a low-memory situation.
 *
 * Return value:
 *  NULL upon error, which is an implied NSS_ERROR_NO_MEMORY
 *  A NON-caller-owned pointer to an array of NSSError values
 */

NSS_EXTERN NSSError *
NSS_GetErrorStack (
  void
);

#define NSSITEM_INIT(it, b, l) \
  (it)->data = (void *)(b);      \
  (it)->size = l;

NSS_EXTERN NSSItem *
NSSItem_Create (
  NSSArena *arenaOpt,
  NSSItem *rvOpt,
  PRUint32 length,
  const void *data
);

NSS_EXTERN void
NSSItem_Destroy (
  NSSItem *item
);

NSS_EXTERN NSSItem *
NSSItem_Duplicate (
  NSSItem *obj,
  NSSArena *arenaOpt,
  NSSItem *rvOpt
);

NSS_EXTERN PRBool
NSSItem_Equal (
  const NSSItem *one,
  const NSSItem *two,
  PRStatus *statusOpt
);

NSS_EXTERN PRStatus
NSSUTF8_Destroy (
  NSSUTF8 *string
);

NSS_EXTERN NSSUTF8 *
NSSUTF8_Duplicate (
  const NSSUTF8 *s,
  NSSArena *arenaOpt
);

/*
 * Functions to start a base64 decoding/encoding context.
 */

NSS_EXTERN NSSBase64Decoder *
NSSBase64Decoder_Create (
  PRInt32 (*output_fn) (void *, const unsigned char *, PRInt32),
  void *output_arg
);

NSS_EXTERN NSSBase64Encoder *
NSSBase64Encoder_Create (
  PRInt32 (*output_fn) (void *, const char *, PRInt32),
  void *output_arg
);

/*
 * Push data through the decoder/encoder, causing the output_fn (provided
 * to Create) to be called with the decoded/encoded data.
 */

NSS_EXTERN PRStatus
NSSBase64Decoder_Update (
  NSSBase64Decoder *data, 
  const char *buffer,
  PRUint32 size
);

NSS_EXTERN PRStatus
NSSBase64Encoder_Update (
  NSSBase64Encoder *data, 
  const unsigned char *buffer,
  PRUint32 size
);

/*
 * When you're done processing, call this to close the context.
 * If "abort_p" is false, then calling this may cause the output_fn
 * to be called one last time (as the last buffered data is flushed out).
 */

NSS_EXTERN PRStatus
NSSBase64Decoder_Destroy (
  NSSBase64Decoder *data, 
  PRBool abort_p
);

NSS_EXTERN PRStatus
NSSBase64Encoder_Destroy (
  NSSBase64Encoder *data, 
  PRBool abort_p
);

/*
 * Perform base64 decoding from an ascii string "inStr" to an Item.
 * The length of the input must be provided as "inLen".  The Item
 * may be provided (as "outItemOpt"); you can also pass in a NULL
 * and the Item will be allocated for you.
 *
 * In any case, the data within the Item will be allocated for you.
 * All allocation will happen out of the passed-in "arenaOpt", if non-NULL.
 * If "arenaOpt" is NULL, standard allocation (heap) will be used and
 * you will want to free the result via SECITEM_FreeItem.
 *
 * Return value is NULL on error, the Item (allocated or provided) otherwise.
 */

NSS_EXTERN NSSItem *
NSSBase64_DecodeBuffer (
  NSSArena *arenaOpt, 
  NSSItem *outItemOpt,
  const NSSUTF8 *inStr, 
  PRUint32 inLen
);

/*
 * Perform base64 encoding of binary data "inItem" to an ascii string.
 * The output buffer may be provided (as "outStrOpt"); you can also pass
 * in a NULL and the buffer will be allocated for you.  The result will
 * be null-terminated, and if the buffer is provided, "maxOutLen" must
 * specify the maximum length of the buffer and will be checked to
 * supply sufficient space space for the encoded result.  (If "outStrOpt"
 * is NULL, "maxOutLen" is ignored.)
 *
 * If "outStrOpt" is NULL, allocation will happen out of the passed-in
 * "arenaOpt", if *it* is non-NULL, otherwise standard allocation (heap)
 * will be used.
 *
 * Return value is NULL on error, the output buffer (allocated or provided)
 * otherwise.
 */

NSS_EXTERN NSSUTF8 *
NSSBase64_EncodeItem (
  NSSArena *arenaOpt, 
  NSSUTF8 *outStrOpt,
  PRUint32 maxOutLen, 
  NSSItem *inItem
);

PR_END_EXTERN_C

#endif /* NSSBASE_H */

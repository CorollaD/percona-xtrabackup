/******************************************************
Copyright (c) 2017, 2020 Percona LLC and/or its affiliates.

Encryption datasink implementation for XtraBackup.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA

*******************************************************/

#include <my_base.h>

#if defined(GCC_VERSION) && (GCC_VERSION >= 4002)
/* Workaround to avoid "gcry_ac_* is deprecated" warnings in gcrypt.h */
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

#include <gcrypt.h>

#ifdef __cplusplus
extern "C" {
#endif

extern char *ds_encrypt_key;
extern char *ds_encrypt_key_file;
extern ulong ds_encrypt_algo;

/******************************************************************************
Utility interface */
bool xb_crypt_read_key_file(const char *filename, void **key, uint *keylength);

void xb_crypt_create_iv(void *ivbuf, size_t ivlen);

/* Initialize libgcrypt */
gcry_error_t xb_libgcrypt_init();

/* Initialize gcrypt and setup encryption key and IV lengths */
gcry_error_t xb_crypt_init(uint *iv_len);

/* Setup gcrypt cipher */
gcry_error_t xb_crypt_cipher_open(gcry_cipher_hd_t *cipher_handle);

/* Close gcrypt cipher */
void xb_crypt_cipher_close(gcry_cipher_hd_t cipher_handle);

/************************************************************************
Mask the argument value. This is to avoid showing secret data on command
line output
@param[in, out]         argument    argument to be masked
@param[in,out]          opt         original argument which is reallocated
*/
void hide_option(char *argument, char **opt);

/* Decrypt buffer */
gcry_error_t xb_crypt_decrypt(gcry_cipher_hd_t cipher_handle, const uchar *from,
                              size_t from_len, uchar *to, size_t *to_len,
                              const uchar *iv, size_t iv_len,
                              bool hash_appended);

/* Encrypt buffer */
gcry_error_t xb_crypt_encrypt(gcry_cipher_hd_t cipher_handle, const uchar *from,
                              size_t from_len, uchar *to, size_t *to_len,
                              uchar *iv);

/* Get the enviorment variable env value into var */
void get_env_value(char *&var, const char *env);

#ifdef __cplusplus
}
#endif

#ifndef __KIWI_H__
#define __KIWI_H__ 1

/* Define Extension Properties */
#define PHP_KIWI_EXTNAME   "Kiwi"
#define PHP_KIWI_EXTVER    "0.0.1"

/* Import configure options
   when building outside of
   the PHP source tree */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"

#include "zend.h"
#include "zend_ini_scanner.h"

#include "string.h"

#ifdef ZTS
#include "TSRM.h"
#endif


ZEND_BEGIN_MODULE_GLOBALS(Kiwi)
  char *test;

  HashTable *dirs;

  char *domain;

  char *pear_dir;
  char *pear_www_dir;
  char *pear_www_vdir;
  char *site_root_dir;
  char *site_root_vdir;
  char *site_private_data_dir;
  char *site_public_data_dir;
  char *site_public_data_vdir;
ZEND_END_MODULE_GLOBALS(Kiwi)



#ifdef ZTS
#define KIWI_G(v) TSRMG(Kiwi_globals_id, zend_Kiwi_globals *, v)
#else
#define KIWI_G(v) (Kiwi_globals.v)
#endif


PHP_MINIT_FUNCTION(Kiwi);
PHP_RINIT_FUNCTION(Kiwi);
PHP_RSHUTDOWN_FUNCTION(Kiwi);
PHP_MSHUTDOWN_FUNCTION(Kiwi);

PHP_FUNCTION(kiwi_base);
PHP_FUNCTION(kiwi_test);
PHP_FUNCTION(kiwi_index);
PHP_FUNCTION(kiwi_admin);
PHP_FUNCTION(kiwi_cron_minuter);
PHP_FUNCTION(kiwi_ajax);

PHP_FUNCTION(kiwi_call);
PHP_FUNCTION(kiwi_call_cn);

PHP_FUNCTION(kiwi_get_domain);
PHP_FUNCTION(kiwi_get_pear_dir);
PHP_FUNCTION(kiwi_get_pear_www_dir);
PHP_FUNCTION(kiwi_get_pear_www_vdir);
PHP_FUNCTION(kiwi_get_site_root_dir);
PHP_FUNCTION(kiwi_get_site_root_vdir);
PHP_FUNCTION(kiwi_get_site_public_data_dir);
PHP_FUNCTION(kiwi_get_site_public_data_vdir);
PHP_FUNCTION(kiwi_get_site_private_data_dir);

static void log_message(char *message);

/* Define the entry point symbol
 * Zend will use when loading this module
 */
extern zend_module_entry Kiwi_module_entry;
#define phpext_Kiwi_ptr &Kiwi_module_entry


#endif

#include "Kiwi.h"



ZEND_DECLARE_MODULE_GLOBALS(Kiwi);

zend_class_entry *php_kiwi_entry;

static zend_function_entry kiwi_functions[] = {
  PHP_FE(kiwi_base, NULL)
  PHP_FE(kiwi_test, NULL)
  PHP_FE(kiwi_index, NULL)
  PHP_FE(kiwi_admin, NULL)
  PHP_FE(kiwi_cron_minuter, NULL)
  PHP_FE(kiwi_ajax, NULL)

  PHP_FE(kiwi_call, NULL)
  PHP_FE(kiwi_call_cn, NULL)

  PHP_FE(kiwi_get_domain, NULL)
  PHP_FE(kiwi_get_pear_dir, NULL)
  PHP_FE(kiwi_get_pear_www_dir, NULL)
  PHP_FE(kiwi_get_pear_www_vdir, NULL)
  PHP_FE(kiwi_get_site_root_dir, NULL)
  PHP_FE(kiwi_get_site_root_vdir, NULL)
  PHP_FE(kiwi_get_site_private_data_dir, NULL)
  PHP_FE(kiwi_get_site_public_data_dir, NULL)
  PHP_FE(kiwi_get_site_public_data_vdir, NULL)

  {NULL, NULL, NULL}
};
static zend_function_entry kiwi_methods[] = {
  PHP_ME_MAPPING(base, kiwi_base, NULL, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
  PHP_ME_MAPPING(test, kiwi_test, NULL, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
  PHP_ME_MAPPING(index, kiwi_index, NULL, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
  PHP_ME_MAPPING(admin, kiwi_admin, NULL, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
  PHP_ME_MAPPING(cronMinuter, kiwi_cron_minuter, NULL, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
  PHP_ME_MAPPING(ajax, kiwi_ajax, NULL, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)

  PHP_ME_MAPPING(call, kiwi_call, NULL, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
  PHP_ME_MAPPING(callCn, kiwi_call_cn, NULL, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)

  PHP_ME_MAPPING(getDomain, kiwi_get_domain, NULL, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
  PHP_ME_MAPPING(getPearDir, kiwi_get_pear_dir, NULL, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
  PHP_ME_MAPPING(getPearWWWDir, kiwi_get_pear_www_dir, NULL, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
  PHP_ME_MAPPING(getPearWWWVDir, kiwi_get_pear_www_vdir, NULL, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
  PHP_ME_MAPPING(getSiteRootDir, kiwi_get_site_root_dir, NULL, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
  PHP_ME_MAPPING(getSiteRootVDir, kiwi_get_site_root_vdir, NULL, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
  PHP_ME_MAPPING(getSitePrivateDataDir, kiwi_get_site_private_data_dir, NULL, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
  PHP_ME_MAPPING(getSitePublicDataDir, kiwi_get_site_public_data_dir, NULL, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
  PHP_ME_MAPPING(getSitePublicDataVDir, kiwi_get_site_public_data_vdir, NULL, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)

  {NULL, NULL, NULL}
};

zend_module_entry Kiwi_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_KIWI_EXTNAME,
    kiwi_functions,
    PHP_MINIT(Kiwi),
    PHP_MSHUTDOWN(Kiwi),
    PHP_RINIT(Kiwi),
    PHP_RSHUTDOWN(Kiwi),
    NULL,
#if ZEND_MODULE_API_NO >= 20010901
    PHP_KIWI_EXTVER,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_KIWI
ZEND_GET_MODULE(Kiwi)
#endif


PHP_INI_BEGIN()
STD_PHP_INI_ENTRY(
  "Kiwi.test", NULL, PHP_INI_ALL, OnUpdateString,
  pear_dir, zend_Kiwi_globals, Kiwi_globals
)
PHP_INI_END()


static void php_kiwi_init_globals(zend_Kiwi_globals *kiwi_globals) {
}
PHP_MINIT_FUNCTION(Kiwi) {

  // инициализируем глобальные переменные
    // функция php_kiwi_init_globals нужна, т.к. если передать NULL, то тут будет seg fault
    ZEND_INIT_MODULE_GLOBALS(Kiwi, php_kiwi_init_globals, NULL);
  ///

  REGISTER_INI_ENTRIES();

  // регистрируем класс Kiwi
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Kiwi", kiwi_methods);
    php_kiwi_entry = zend_register_internal_class(&ce TSRMLS_CC);
  ///


  KIWI_G(dirs) = (HashTable *)pemalloc(sizeof(HashTable), 1);
  zend_hash_init(KIWI_G(dirs), 8, NULL, NULL, 1);

  return SUCCESS;
}
PHP_RINIT_FUNCTION(Kiwi) {

  zval **array;
  zval **token;

  zend_is_auto_global("_SERVER", sizeof("_SERVER")-1 TSRMLS_CC);
  if (zend_hash_find(&EG(symbol_table), "_SERVER", sizeof("_SERVER"), (void **) &array) == SUCCESS &&
      Z_TYPE_PP(array) == IS_ARRAY &&
      zend_hash_find(Z_ARRVAL_PP(array), "SERVER_NAME", sizeof("SERVER_NAME"), (void **) &token) == SUCCESS
      ) {
    KIWI_G(domain) = estrdup(Z_STRVAL_PP(token));
  }
  else {
    KIWI_G(domain) = estrdup("");
  }


  char str[255];
  HashTable *hht;
  char *ss;
  zval tmpzval;

  char dir_str[64];

  if (zend_hash_find(KIWI_G(dirs), KIWI_G(domain), strlen(KIWI_G(domain))+1, (void **)&hht) != SUCCESS) {
    hht = (HashTable *)pemalloc(sizeof(HashTable), 1);
    zend_hash_init(hht, 8, NULL, NULL, 1);
    zend_hash_add(hht, KIWI_G(domain), strlen(KIWI_G(domain))+1, (void *)hht, sizeof(hht), NULL);
  }

  strcpy(dir_str,"pear_dir");
  if (zend_hash_find(hht, dir_str, strlen(dir_str)+1, (void **)&ss) == SUCCESS) {
    KIWI_G(pear_dir) = estrdup(ss);
  }
  else {
    strcpy(str, "Kiwi."), strcat(str, KIWI_G(domain)), strcat(str, "."); strcat(str, dir_str);
    if (
      (SUCCESS == zend_get_configuration_directive(str, strlen(str)+1, &tmpzval)) &&
      (Z_TYPE(tmpzval) == IS_STRING) &&
      (Z_STRLEN(tmpzval) > 0)
    ) {
      KIWI_G(pear_dir) = estrdup(Z_STRVAL(tmpzval));
    }
    else {
      KIWI_G(pear_dir) = estrdup("");
    }
    zend_hash_add(hht, KIWI_G(domain), strlen(KIWI_G(domain))+1, (void *)Z_STRVAL(tmpzval), Z_STRLEN(tmpzval)+1, NULL);
  }


  strcpy(dir_str,"pear_www_dir");
    if (zend_hash_find(hht, dir_str, strlen(dir_str)+1, (void **)&ss) == SUCCESS) {
      KIWI_G(pear_dir) = estrdup(ss);
    }
    else {
      strcpy(str, "Kiwi."), strcat(str, KIWI_G(domain)), strcat(str, "."); strcat(str, dir_str);
      if (
        (SUCCESS == zend_get_configuration_directive(str, strlen(str)+1, &tmpzval)) &&
        (Z_TYPE(tmpzval) == IS_STRING) &&
        (Z_STRLEN(tmpzval) > 0)
      ) {
        KIWI_G(pear_www_dir) = estrdup(Z_STRVAL(tmpzval));
      }
      else {
        KIWI_G(pear_www_dir) = estrdup("");
      }
      zend_hash_add(hht, KIWI_G(domain), strlen(KIWI_G(domain))+1, (void *)Z_STRVAL(tmpzval), Z_STRLEN(tmpzval)+1, NULL);
    }



    strcpy(dir_str,"pear_www_vdir");
      if (zend_hash_find(hht, dir_str, strlen(dir_str)+1, (void **)&ss) == SUCCESS) {
        KIWI_G(pear_dir) = estrdup(ss);
      }
      else {
        strcpy(str, "Kiwi."), strcat(str, KIWI_G(domain)), strcat(str, "."); strcat(str, dir_str);
        if (
          (SUCCESS == zend_get_configuration_directive(str, strlen(str)+1, &tmpzval)) &&
          (Z_TYPE(tmpzval) == IS_STRING) &&
          (Z_STRLEN(tmpzval) > 0)
        ) {
          KIWI_G(pear_www_vdir) = estrdup(Z_STRVAL(tmpzval));
        }
        else {
          KIWI_G(pear_www_vdir) = estrdup("");
        }
        zend_hash_add(hht, KIWI_G(domain), strlen(KIWI_G(domain))+1, (void *)Z_STRVAL(tmpzval), Z_STRLEN(tmpzval)+1, NULL);
      }


  strcpy(dir_str,"site_root_dir");
  if (zend_hash_find(hht, dir_str, strlen(dir_str)+1, (void **)&ss) == SUCCESS) {
    KIWI_G(site_root_dir) = estrdup(ss);
  }
  else {
    strcpy(str, "Kiwi."), strcat(str, KIWI_G(domain)), strcat(str, "."); strcat(str, dir_str);
    if (
      (SUCCESS == zend_get_configuration_directive(str, strlen(str)+1, &tmpzval)) &&
      (Z_TYPE(tmpzval) == IS_STRING) &&
      (Z_STRLEN(tmpzval) > 0)
    ) {
      KIWI_G(site_root_dir) = estrdup(Z_STRVAL(tmpzval));
    }
    else {
      KIWI_G(site_root_dir) = estrdup("");
    }
    zend_hash_add(hht, KIWI_G(domain), strlen(KIWI_G(domain))+1, (void *)Z_STRVAL(tmpzval), Z_STRLEN(tmpzval)+1, NULL);
  }

  strcpy(dir_str,"site_root_vdir");
  if (zend_hash_find(hht, dir_str, strlen(dir_str)+1, (void **)&ss) == SUCCESS) {
    KIWI_G(site_root_vdir) = estrdup(ss);
  }
  else {
    strcpy(str, "Kiwi."), strcat(str, KIWI_G(domain)), strcat(str, "."); strcat(str, dir_str);
    if (
      (SUCCESS == zend_get_configuration_directive(str, strlen(str)+1, &tmpzval)) &&
      (Z_TYPE(tmpzval) == IS_STRING) &&
      (Z_STRLEN(tmpzval) > 0)
    ) {
      KIWI_G(site_root_vdir) = estrdup(Z_STRVAL(tmpzval));
    }
    else {
      KIWI_G(site_root_vdir) = estrdup("");
    }
    zend_hash_add(hht, KIWI_G(domain), strlen(KIWI_G(domain))+1, (void *)Z_STRVAL(tmpzval), Z_STRLEN(tmpzval)+1, NULL);
  }




  strcpy(dir_str,"site_private_data_dir");
  if (zend_hash_find(hht, dir_str, strlen(dir_str)+1, (void **)&ss) == SUCCESS) {
    KIWI_G(site_private_data_dir) = estrdup(ss);
  }
  else {
    strcpy(str, "Kiwi."), strcat(str, KIWI_G(domain)), strcat(str, "."); strcat(str, dir_str);
    if (
      (SUCCESS == zend_get_configuration_directive(str, strlen(str)+1, &tmpzval)) &&
      (Z_TYPE(tmpzval) == IS_STRING) &&
      (Z_STRLEN(tmpzval) > 0)
    ) {
      KIWI_G(site_private_data_dir) = estrdup(Z_STRVAL(tmpzval));
    }
    else {
      KIWI_G(site_private_data_dir) = estrdup("");
    }
    zend_hash_add(hht, KIWI_G(domain), strlen(KIWI_G(domain))+1, (void *)Z_STRVAL(tmpzval), Z_STRLEN(tmpzval)+1, NULL);
  }


  strcpy(dir_str,"site_public_data_dir");
  if (zend_hash_find(hht, dir_str, strlen(dir_str)+1, (void **)&ss) == SUCCESS) {
    KIWI_G(site_public_data_dir) = estrdup(ss);
  }
  else {
    strcpy(str, "Kiwi."), strcat(str, KIWI_G(domain)), strcat(str, "."); strcat(str, dir_str);
    if (
      (SUCCESS == zend_get_configuration_directive(str, strlen(str)+1, &tmpzval)) &&
      (Z_TYPE(tmpzval) == IS_STRING) &&
      (Z_STRLEN(tmpzval) > 0)
    ) {
      KIWI_G(site_public_data_dir) = estrdup(Z_STRVAL(tmpzval));
    }
    else {
      KIWI_G(site_public_data_dir) = estrdup("");
    }
    zend_hash_add(hht, KIWI_G(domain), strlen(KIWI_G(domain))+1, (void *)Z_STRVAL(tmpzval), Z_STRLEN(tmpzval)+1, NULL);
  }




  strcpy(dir_str,"site_public_data_vdir");
  if (zend_hash_find(hht, dir_str, strlen(dir_str)+1, (void **)&ss) == SUCCESS) {
    KIWI_G(site_public_data_vdir) = estrdup(ss);
  }
  else {
    strcpy(str, "Kiwi."), strcat(str, KIWI_G(domain)), strcat(str, "."); strcat(str, dir_str);
    if (
      (SUCCESS == zend_get_configuration_directive(str, strlen(str)+1, &tmpzval)) &&
      (Z_TYPE(tmpzval) == IS_STRING) &&
      (Z_STRLEN(tmpzval) > 0)
    ) {
      KIWI_G(site_public_data_vdir) = estrdup(Z_STRVAL(tmpzval));
    }
    else {
      KIWI_G(site_public_data_vdir) = estrdup("");
    }
    zend_hash_add(hht, KIWI_G(domain), strlen(KIWI_G(domain))+1, (void *)Z_STRVAL(tmpzval), Z_STRLEN(tmpzval)+1, NULL);
  }

  return SUCCESS;
}
PHP_RSHUTDOWN_FUNCTION(Kiwi) {
  efree(KIWI_G(domain));
  efree(KIWI_G(pear_dir));
  //efree(KIWI_G(site_root_dir));
  return SUCCESS;
}
PHP_MSHUTDOWN_FUNCTION(Kiwi) {
  zend_hash_destroy(KIWI_G(dirs));
  pefree(KIWI_G(dirs), 1);
  UNREGISTER_INI_ENTRIES();
  return SUCCESS;
}

PHP_FUNCTION(kiwi_base) {
  char eval_str[255];
  strcpy(eval_str, "require_once('");
  if ((KIWI_G(pear_dir) != NULL) && (strlen(KIWI_G(pear_dir)) > 0)) {
    strcat(eval_str, KIWI_G(pear_dir));
    strcat(eval_str, "/php/");
  }
  strcat(eval_str, "Kiwi/Base/Autoloader.php');");
  strcat(eval_str, "Kiwi_Base_Autoloader::register();");
  strcat(eval_str, "Kiwi_Base::init();");

  zend_eval_string(eval_str, NULL, "Kiwi::base()" TSRMLS_CC);
  RETURN_TRUE;
}
PHP_FUNCTION(kiwi_test) {
  char eval_str[255];
  strcpy(eval_str, "require_once('");
  if ((KIWI_G(pear_dir) != NULL) && (strlen(KIWI_G(pear_dir)) > 0)) {
    strcat(eval_str, KIWI_G(pear_dir));
    strcat(eval_str, "/php/");
  }
  strcat(eval_str, "Kiwi/Base/Autoloader.php');");
  strcat(eval_str, "Kiwi_Base_Autoloader::register();");
  strcat(eval_str, "Kiwi_Base::init();");
  strcat(eval_str, "Kiwi_Test::test();");

  zend_eval_string(eval_str, NULL, "Kiwi::test()" TSRMLS_CC);
  RETURN_TRUE;
}
PHP_FUNCTION(kiwi_index) {
  char eval_str[255];
  strcpy(eval_str, "require_once('");
  if ((KIWI_G(pear_dir) != NULL) && (strlen(KIWI_G(pear_dir)) > 0)) {
    strcat(eval_str, KIWI_G(pear_dir));
    strcat(eval_str, "/php/");
  }
  strcat(eval_str, "Kiwi/Base/Autoloader.php');");
  strcat(eval_str, "Kiwi_Base_Autoloader::register();");
  strcat(eval_str, "Kiwi_Base::init();");
  strcat(eval_str, "Kiwi_Base::index();");

  zend_eval_string(eval_str, NULL, "Kiwi::index()" TSRMLS_CC);
  RETURN_TRUE;
}
PHP_FUNCTION(kiwi_admin) {
  char eval_str[255];
  strcpy(eval_str, "require_once('");
  if ((KIWI_G(pear_dir) != NULL) && (strlen(KIWI_G(pear_dir)) > 0)) {
    strcat(eval_str, KIWI_G(pear_dir));
    strcat(eval_str, "/php/");
  }
  strcat(eval_str, "Kiwi/Base/Autoloader.php');");
  strcat(eval_str, "Kiwi_Base_Autoloader::register();");
  strcat(eval_str, "Kiwi_Base::init();");
  strcat(eval_str, "Kiwi_Base_Admin::index();");

  zend_eval_string(eval_str, NULL, "Kiwi::admin()" TSRMLS_CC);
  RETURN_TRUE;
}
PHP_FUNCTION(kiwi_cron_minuter) {
  char eval_str[255];
  strcpy(eval_str, "require_once('");
  if ((KIWI_G(pear_dir) != NULL) && (strlen(KIWI_G(pear_dir)) > 0)) {
    strcat(eval_str, KIWI_G(pear_dir));
    strcat(eval_str, "/php/");
  }
  strcat(eval_str, "Kiwi/Base/Autoloader.php');");
  strcat(eval_str, "Kiwi_Base_Autoloader::register();");
  strcat(eval_str, "Kiwi_Base::init();");
  strcat(eval_str, "Kiwi_Base::cronMinuter();");

  zend_eval_string(eval_str, NULL, "Kiwi::cronMinuter()" TSRMLS_CC);
  RETURN_TRUE;
}
PHP_FUNCTION(kiwi_ajax) {
  char eval_str[255];
  strcpy(eval_str, "require_once('");
  if ((KIWI_G(pear_dir) != NULL) && (strlen(KIWI_G(pear_dir)) > 0)) {
    strcat(eval_str, KIWI_G(pear_dir));
    strcat(eval_str, "/php/");
  }
  strcat(eval_str, "Kiwi/Base/Autoloader.php');");
  strcat(eval_str, "Kiwi_Base_Autoloader::register();");
  strcat(eval_str, "Kiwi_Base::init();");
  strcat(eval_str, "Kiwi_Ajax::ajax();");

  zend_eval_string(eval_str, NULL, "Kiwi::ajax()" TSRMLS_CC);
  RETURN_TRUE;
}


PHP_FUNCTION(kiwi_call) {
  zval *params, *retval_ptr = NULL;
  zend_fcall_info fci;
  zend_fcall_info_cache fci_cache;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "f|a/", &fci, &fci_cache, &params) == FAILURE) {
    return;
  }

  zend_fcall_info_args(&fci, params TSRMLS_CC);
  fci.retval_ptr_ptr = &retval_ptr;

  if (zend_call_function(&fci, &fci_cache TSRMLS_CC) == SUCCESS && fci.retval_ptr_ptr && *fci.retval_ptr_ptr) {
    COPY_PZVAL_TO_ZVAL(*return_value, *fci.retval_ptr_ptr);
  }

  zend_fcall_info_args_clear(&fci, 1);
}
PHP_FUNCTION(kiwi_call_cn) {
  zval *params, *retval_ptr = NULL;
  zend_fcall_info fci;
  zend_fcall_info_cache fci_cache;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sf|a/", &fci, &fci_cache, &params) == FAILURE) {
    return;
  }

  zend_fcall_info_args(&fci, params TSRMLS_CC);
  fci.retval_ptr_ptr = &retval_ptr;

  if (zend_call_function(&fci, &fci_cache TSRMLS_CC) == SUCCESS && fci.retval_ptr_ptr && *fci.retval_ptr_ptr) {
    COPY_PZVAL_TO_ZVAL(*return_value, *fci.retval_ptr_ptr);
  }

  zend_fcall_info_args_clear(&fci, 1);
}



PHP_FUNCTION(kiwi_get_domain) {
  RETURN_STRING(KIWI_G(domain), 1);
}
PHP_FUNCTION(kiwi_get_pear_dir) {
  RETURN_STRING(KIWI_G(pear_dir),1);
}
PHP_FUNCTION(kiwi_get_pear_www_dir) {
  RETURN_STRING(KIWI_G(pear_www_dir),1);
}
PHP_FUNCTION(kiwi_get_pear_www_vdir) {
  RETURN_STRING(KIWI_G(pear_www_vdir),1);
}
PHP_FUNCTION(kiwi_get_site_root_dir) {
  RETURN_STRING(KIWI_G(site_root_dir),1);
}
PHP_FUNCTION(kiwi_get_site_root_vdir) {
  RETURN_STRING(KIWI_G(site_root_vdir),1);
}
PHP_FUNCTION(kiwi_get_site_private_data_dir) {
  RETURN_STRING(KIWI_G(site_private_data_dir),1);
}
PHP_FUNCTION(kiwi_get_site_public_data_dir) {
  RETURN_STRING(KIWI_G(site_public_data_dir),1);
}
PHP_FUNCTION(kiwi_get_site_public_data_vdir) {
  RETURN_STRING(KIWI_G(site_public_data_vdir),1);
}


static void log_message(char *message)
{
    FILE *log;
    log = fopen("/home/hw/kiwi.so.log", "w");
    fprintf (log, "%s\n", message);
    fclose(log);
}



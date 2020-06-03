require 'mkmf'

extension_name = 'ssl_stat'
dir_config extension_name

$LOCAL_LIBS << " -lcurl"

create_makefile extension_name
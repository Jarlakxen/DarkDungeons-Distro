/*
 * $Id: settings.inc 583 2006-07-05 11:01:40Z austinheilman $
 *
 */

use uo;
use os;
use cfgfile;

/*
 * HS_GetSettingsCfgFile(engine_name)
 *
 * Purpose
 * Reads in :help:settings.cfg
 *
 * Parameters
 *
 * Return value
 * A config file reference.
 *
 */
function HS_GetSettingsCfgFile()
	var cfg := ReadConfigFile(":help:settings");

	if ( cfg.errortext )
		SysLog("Error::HS_GetSettingsCfgFile() - Unable to open [:help:settings.cfg] ->"+cfg.errortext);
	endif

	return cfg;
endfunction

/*
 * HS_GetSettingsCfgElem(elem_name, cfg_file)
 *
 * Purpose
 * Retrieves an elem from a config file.
 *
 * Parameters
 * elem_name:	A string matching the elem name to be retrieved.
 * cfg_file:	Optional parameter - reference to a config already read in by HS_GetSettingsCfgFile()
 *
 * Return value
 * A config file elem reference.
 *
 */
function HS_GetSettingsCfgElem(elem_name, byref cfg_file:=0)
	if ( !cfg_file )
		cfg_file := HS_GetSettingsCfgFile();
	endif

	var elem := cfg_file[elem_name];

	if ( elem.errortext )
		SysLog("Error::HS_GetSettingsCfgElem() - Unable to find elem ["+elem_name+"] ->"+elem.errortext);
	endif

	return elem;
endfunction
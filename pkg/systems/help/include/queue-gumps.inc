/* $Id: queue-gumps.inc 797 2006-09-12 20:54:37Z austinheilman $
 *
 */

use uo;
use os;
use util;

include ":gumps:gumps";
include ":gumps:gumps_ex";
include ":gumps:gumpprompt";
include ":help:help";

CONST CLOSE_BTN		:= 0xA00;
CONST QUEUE_TYPES_BTN	:= 0xA01;
CONST PAGE_LIST_BTN	:= 0xA02;
CONST SHOW_PAGE_BTN	:= 0xA03;
CONST DELETE_PAGE_BTN	:= 0xA04;
CONST GOTO_BTN		:= 0xA06;
CONST SUMMON_BTN	:= 0xA07;
CONST INFO_BTN		:= 0xA08;
CONST ACCTINFO_BTN	:= 0xA09;
CONST EMAIL_BTN		:= 0xA0A;

CONST SHOW_QUEUE_START	:= 0xA000;
CONST SHOW_PAGE_START	:= 0xB000;

function BuildGumpTemplate()
	var gump := LoadHelpInfo("QueueTemplate", "Gump");

	if ( gump )
		return gump;
	endif

	gump := GFCreateGump();
	GFResizePic(gump, 0, 0, GFGetCfgConst("Defaults", "BackGround"), 540, 450);
	GFResizePic(gump, 15, 15, GFGetCfgConst("Defaults", "ForeGround"), 510, 25);
	GFTextMid(gump, 15, 20, 570, 2100, "Help Queue");

	// Left bar
	GFResizePic(gump, 15, 45, GFGetCfgConst("Defaults", "ForeGround"), 125, 325);

	// Content Area
	GFResizePic(gump, 145, 45, GFGetCfgConst("Defaults", "ForeGround"), 380, 325);

	// Bar along the bottom
	GFResizePic(gump, 15, 375, GFGetCfgConst("Defaults", "ForeGround"), 510, 60);

	return gump;
endfunction

function ShowQueueTypes(cmdlevel)
	var gump := BuildGumpTemplate();
	var y_pos := 50;
	foreach level in ( GetQueueNamesAccess(cmdlevel) )
		GFAddButton(gump, 20, y_pos+3, 2117, 2118, GF_CLOSE_BTN, SHOW_QUEUE_START+_level_iter);
		GFTextLine(gump, 40, y_pos, 2100, level);
		y_pos += 20;
		SleepMS(2);
	endforeach

	return gump;
endfunction

function ShowPagesInQueue(queue_name, cmd_level)
	var gump := ShowQueueTypes(cmd_level);

	var data_file := GetHelpDataFile();
	var data_elem := DFFindElement(data_file, "Queue-"+queue_name, DF_NO_CREATE);

	var y_pos := 50;
	var prop_names := data_elem.PropNames();
	var counter := 0, num_pages := prop_names.Size();

	GFPage(gump, 1);
	foreach prop_name in ( prop_names )
		var page_info := data_elem.GetProp(prop_name);
		var mobile := SystemFindObjectBySerial(CInt(prop_name));

		var status := "(online)";
		if ( !mobile.connected )
			status := "(offline)";
		endif

		GFAddButton(gump, 150, y_pos+3, 2117, 2118, GF_CLOSE_BTN, SHOW_PAGE_START+_prop_name_iter);
		GFTextLine(gump, 170, y_pos, 2100, mobile.name+" "+status);
		var summary;
		if ( Len(page_info.message) > 45 )
			summary := page_info.message[1, 45]+"...";
		else
			summary := page_info.message;
		endif
		GFTextLine(gump, 170, y_pos+20, 2100, summary);

		counter += 1;
		y_pos += 45;

		if ( counter >= 7 && _prop_name_iter < num_pages )
			GFAddButton(gump, 510, 345, 0xA92, 0xA93, GF_PAGE_BTN, gump.cur_page+1);
			GFPage(gump, gump.cur_page+1);
			GFAddButton(gump, 510, 50, 0xA90, 0xA91, GF_PAGE_BTN, gump.cur_page-1);
			counter := 0;
			y_pos := 50;
		endif

		SleepMS(2);
	endforeach

	return gump;
endfunction

function ShowPageInQueue(queue_name, page_id, cmdlevel)
	var data_file := GetHelpDataFile();
	var data_elem := DFFindElement(data_file, "Queue-"+queue_name, DF_NO_CREATE);
	var page_info := data_elem.GetProp(Hex(CInt(page_id)));

	var mobile := SystemFindObjectBySerial(CInt(page_id));

	var gump := ShowQueueTypes(cmdlevel);

	var status := "(online)";
	if ( !mobile.connected )
		status := "(offline)";
	endif
	GFTextLine(gump, 150, 50, 2100, "Viewing Page From: "+mobile.name+" "+status);
	GFTextLine(gump, 150, 70, 2100, "Page Time: "+StrFormatTime("%b %d, %Y %I:%M %p", page_info.time));


	GFResizePic(gump, 150, 90, GFGetCfgConst("BackGrounds", "GoldTrim"), 370, 270);
	var y_pos := 95;
	foreach line in ( GFWordWrap(page_info.message, 370) )
		GFTextLine(gump, 155, y_pos, 2100, line);
		y_pos += 20;
		SleepMS(2);
	endforeach

	var btn_type := GF_DUMMY_BTN;
	if ( cmdlevel >= 3 )
		btn_type := GF_CLOSE_BTN;
	endif

	GFAddButton(gump, 20, 385, 2117, 2118, btn_type, GOTO_BTN);
	GFTextLine(gump, 40, 382, 2100, "Goto");
	GFAddButton(gump, 20, 413, 2117, 2118, btn_type, SUMMON_BTN);
	GFTextLine(gump, 40, 410, 2100, "Summon");

	GFAddButton(gump, 160, 385, 2117, 2118, btn_type, INFO_BTN);
	GFTextLine(gump, 180, 382, 2100, "Character Info");
	GFAddButton(gump, 160, 413, 2117, 2118, btn_type, ACCTINFO_BTN);
	GFTextLine(gump, 180, 410, 2100, "Account Info");

	GFAddButton(gump, 300, 385, 2117, 2118, GF_CLOSE_BTN, EMAIL_BTN);
	GFTextLine(gump, 320, 382, 2100, "Send Email");
	GFAddButton(gump, 300, 413, 2117, 2118, GF_CLOSE_BTN, DELETE_PAGE_BTN);
	GFTextLine(gump, 320, 410, 2100, "Delete Page");

	return gump;
endfunction

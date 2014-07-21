/* vim:set et sts=4: */

#include <string.h>
#include <syslog.h>
#include "convert.h"

typedef struct {
	char pre_str[4];
	char aft_str[64];
	int replace_size;
} CONV_INFO;

#define MAX_IDXLEN_ROMCANA (3)
static const CONV_INFO RomCana[] = {
	{"a", "あ", 1}, {"i", "い", 1}, {"u", "う", 1}, {"e", "え", 1}, {"o", "お", 1},
	{"ka", "か", 2}, {"ki", "き", 2}, {"ku", "く", 2}, {"ke", "け", 2}, {"ko", "こ", 2},
	{"sa", "さ", 2}, {"si", "し", 2}, {"su", "す", 2}, {"se", "せ", 2}, {"so", "そ", 2},
	{"ta", "た", 2}, {"ti", "ち", 2}, {"tu", "つ", 2}, {"te", "て", 2}, {"to", "と", 2},
	{"na", "な", 2}, {"ni", "に", 2}, {"nu", "ぬ", 2}, {"ne", "ね", 2}, {"no", "の", 2},
	{"ha", "は", 2}, {"hi", "ひ", 2}, {"hu", "ふ", 2}, {"he", "へ", 2}, {"ho", "ほ", 2},
	{"ma", "ま", 2}, {"mi", "み", 2}, {"mu", "む", 2}, {"me", "め", 2}, {"mo", "も", 2},
	{"ya", "や", 2}, {"yi", "い", 2}, {"yu", "ゆ", 2}, {"ye", "いぇ", 2}, {"yo", "よ", 2},
	{"ra", "ら", 2}, {"ri", "り", 2}, {"ru", "る", 2}, {"re", "れ", 2}, {"ro", "ろ", 2},
	{"wa", "わ", 2}, {"wi", "うぃ", 2}, {"wu", "う", 2}, {"we", "うぇ", 2}, {"wo", "を", 2},
	{"la", "ぁ", 2}, {"li", "ぃ", 2}, {"lu", "ぅ", 2}, {"le", "ぇ", 2}, {"lo", "ぉ", 2},
	{"ga", "が", 2}, {"gi", "ぎ", 2}, {"gu", "ぐ", 2}, {"ge", "げ", 2}, {"go", "ご", 2},
	{"za", "ざ", 2}, {"zi", "じ", 2}, {"zu", "ず", 2}, {"ze", "ぜ", 2}, {"zo", "ぞ", 2},
	{"da", "だ", 2}, {"di", "ぢ", 2}, {"du", "づ", 2}, {"de", "で", 2}, {"do", "ど", 2},
	{"ba", "ば", 2}, {"bi", "び", 2}, {"bu", "ぶ", 2}, {"be", "べ", 2}, {"bo", "ぼ", 2},
	{"pa", "ぱ", 2}, {"pi", "ぴ", 2}, {"pu", "ぷ", 2}, {"pe", "ぺ", 2}, {"po", "ぽ", 2},
	{"kya", "きゃ", 3}, {"kyi", "きぃ", 3}, {"kyu", "きゅ", 3}, {"kye", "きぇ", 3}, {"kyo", "きょ", 3},
	{"sya", "しゃ", 3}, {"syi", "しぃ", 3}, {"syu", "しゅ", 3}, {"sye", "しぇ", 3}, {"syo", "しょ", 3},
	{"tya", "ちゃ", 3}, {"tyi", "ちぃ", 3}, {"tyu", "ちゅ", 3}, {"tye", "ちぇ", 3}, {"tyo", "ちょ", 3},
	{"nya", "にゃ", 3}, {"nyi", "にぃ", 3}, {"nyu", "にゅ", 3}, {"nye", "にぇ", 3}, {"nyo", "にょ", 3},
	{"hya", "ひゃ", 3}, {"hyi", "ひぃ", 3}, {"hyu", "ひゅ", 3}, {"hye", "ひぇ", 3}, {"hyo", "ひょ", 3},
	{"mya", "みゃ", 3}, {"myi", "みぃ", 3}, {"myu", "みゅ", 3}, {"mye", "みぇ", 3}, {"myo", "みょ", 3},
	{"rya", "りゃ", 3}, {"ryi", "りぃ", 3}, {"ryu", "りゅ", 3}, {"rye", "りぇ", 3}, {"ryo", "りょ", 3},
	{"lya", "ゃ", 3}, {"lyi", "ぃ", 3}, {"lyu", "ゅ", 3}, {"lye", "ぇ", 3}, {"lyo", "ょ", 3}, {"ltu", "っ", 3},
	{"bb", "っ", 1}, {"cc", "っ", 1}, {"dd", "っ", 1}, {"ff", "っ", 1}, {"gg", "っ", 1}, {"hh", "っ", 1},
	{"jj", "っ", 1}, {"kk", "っ", 1}, {"mm", "っ", 1}, {"pp", "っ", 1}, {"rr", "っ", 1}, {"ss", "っ", 1},
	{"tt", "っ", 1}, {"vv", "っ", 1}, {"ww", "っ", 1}, {"xx", "っ", 1}, {"yy", "っ", 1}, {"zz", "っ", 1},
	{"nn", "ん", 2}
};
#define ROMCANA_TBL_SIZE (sizeof(RomCana) / sizeof(CONV_INFO))


int conv_run_romajiconv(GString *gStr, gssize curpos)
{
	int idx_len = MAX_IDXLEN_ROMCANA;
	int i, j;
	gssize conv_pos = 0;
	gchar *search_p = NULL;
	
	if (curpos < MAX_IDXLEN_ROMCANA) {
		idx_len = curpos + 1;
	}

	for (i = idx_len; 0 < i; i--) {
		conv_pos = curpos - i + 1;
		search_p = gStr->str + conv_pos;

		for (j = 0; j < ROMCANA_TBL_SIZE; j++) {
			if (strlen(RomCana[j].pre_str) == i) {
				if ((strncmp(search_p, RomCana[j].pre_str, i)) == 0) {
					g_string_erase (gStr, conv_pos, RomCana[j].replace_size);
					g_string_insert (gStr, conv_pos, RomCana[j].aft_str);
					return sizeof(RomCana[j].aft_str);
				}
			}

 		}
	}
	
	return 1;
}

/* for UTF-8 (only) */
int conv_get_bytesize_laststr(GString *gStr, gssize curpos)
{
	int i;
	
	for (i = 1; i <= curpos; i++) {
		if ((gStr->str[curpos - i]) & 0x40) {
			break;
		}
	}
	
	return i;
}
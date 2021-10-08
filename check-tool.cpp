#include "my-gists/ukagaka/shiori_loader.hpp"
#include "my-gists/codepage.hpp"
#include <cstdlib>
#include <cstdio>
using namespace CODEPAGE_n;

Cshiori shiori;

#define	E_I			0	/* info */
#define	E_F			1	/* fatal */
#define	E_E			2	/* error */
#define	E_W			3	/* warning */
#define	E_N			4	/* note */
#define E_J			5	/* other(j) */
#define	E_END		6	/* ログの終了 */
#define	E_SJIS		16	/* マルチバイト文字コード＝SJIS */
#define	E_UTF8		17	/* マルチバイト文字コード＝UTF-8 */
#define	E_DEFAULT	32	/* マルチバイト文字コード＝OSデフォルトのコード */
CODEPAGE the_using=CP_UTF8;
std::wstring do_charset_convert(std::wstring_view a) {
	return the_using==CP_UTF8?(std::wstring)a:MultiByteToUnicode(UnicodeToMultiByte(a.data(),the_using),CP_UTF8);
}

void loghandler(const wchar_t *str, int mode){
	auto tmp = do_charset_convert(str);
	str=tmp.c_str();
	switch(mode){
		case E_SJIS:
			the_using=CP_SJIS;
			break;
		case E_UTF8:
			the_using=CP_UTF8;
			break;
		case E_DEFAULT:
			the_using=CP_ACP;
			break;
		case E_END:
			break;
		case E_I:/* info */
			fwprintf(stdout,str);
			break;
		case E_F:/* fatal */
			fprintf(stderr,"::error title=fatal::fatal:%ls",str);
			break;
		case E_E:/* error */
			fprintf(stderr,"::error title=error::error:%ls",str);
			break;
		case E_W:/* warning */
			fprintf(stderr,"::warning title=warning::warning:%ls",str);
			break;
		case E_N:/* note */
			fprintf(stderr,"::notice title=notice::notice:%ls",str);
			break;
		case E_J:/* other(j) */
			fwprintf(stdout,str);
			break;
	};
}
int wmain(int argc,wchar_t**argv){
	//::error file={name},line={line},endLine={endLine},title={title}::{message}
	//::warning file={name},line={line},endLine={endLine},title={title}::{message}
	//::notice file={name},line={line},endLine={endLine},title={title}::{message}
	//
	shiori.SetTo(argv[1]);
	if(!shiori.All_OK())
		fprintf(stderr,"::error title=shiori.ALL_OK() returns false::shiori load failed");
	if(!shiori.can_make_CI_check())
		fprintf(stderr,"::error title=checker is NULL::Unsupported shiori");
	auto failed=shiori.CI_check_failed();
	if (failed){
		fprintf(stderr,"::error title=open your tama!::some error in your dic");
	}
	return failed?EXIT_FAILURE:EXIT_SUCCESS;
};

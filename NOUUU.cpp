// NOUUU.cpp : Defines the entry point for the application.
//
#include "stdafx.h"

#include<windows.h>
#include<winresrc.h>
#include <commdlg.h>
#include<mmsystem.h>
#include<string.h>
#include<cstdio>
#include<stdlib.h>
#include<wchar.h>
#include<time.h>
#include "NOUUU.h"
#include "Resource.h"
//#include "atlbase.h"
using namespace::std;
#define MAX_LOADSTRING 100
// Global Variables:
char nume_fis[100];
char fisintrare[MAX_PATH];
char fisiesire1[MAX_PATH];
char fisiesire2[MAX_PATH];
int len;
int len1;
int len2;
int len3;
TCHAR FULL[MAX_PATH + 11];


#define Code_value_bits 16              /* Number of bits in a code value   */
typedef long code_value;                /* Type of an arithmetic code value */

#define Top_value (((long)1<<Code_value_bits)-1)      /* Largest code value */


#define First_qtr (Top_value/4+1)       /* Point after first quarter        */
#define Half      (2*First_qtr)         /* Point after first half           */
#define Third_qtr (3*First_qtr)         /* Point after third quarter        */

#define No_of_chars 256                 /* Number of character symbols      */
#define EOF_symbol (No_of_chars+1)      /* Index of EOF symbol              */

#define No_of_symbols (No_of_chars+1)   /* Total number of symbols          */

										/* TRANSLATION TABLES BETWEEN CHARACTERS AND SYMBOL INDEXES. */

int char_to_index[No_of_chars];         /* To index from character          */
unsigned char index_to_char[No_of_symbols + 1]; /* To character from index    */

												/* CUMULATIVE FREQUENCY TABLE. */

#define Max_frequency 16383             /* Maximum allowed frequency count */
												/*   2^14 - 1                       */
int cum_freq[No_of_symbols + 1];          /* Cumulative symbol frequencies    */

										  //
int freq[No_of_symbols + 1] = {
	0,
	1,   1,   1,   1,   1,   1,   1,   1,   1,   1, 124,   1,   1,   1,   1,   1,
	1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,

	/*      !    "    #    $    %    &    '    (    )    *    +    ,    -    .    / */
	1236,   1, 21,   9,   3,   1, 25, 15,   2,   2,   2,   1, 79, 19, 60,   1,

	/* 0    1    2    3    4    5    6    7    8    9    :    ;    <    =    >    ? */
	15, 15,   8,   5,   4,   7,   5,   4,   4,   6,   3,   2,   1,   1,   1,   1,

	/* @    A    B    C    D    E    F    G    H    I    J    K    L    M    N    O */
	1, 24, 15, 22, 12, 15, 10,   9, 16, 16,   8,   6, 12, 23, 13, 11,

	/* P    Q    R    S    T    U    V    W    X    Y    Z    [    /    ]    ^    _ */
	14,   1, 14, 28, 29,   6,   3, 11,   1,   3,   1,   1,   1,   1,   1,   3,

	/* '    a    b    c    d    e    f    g    h    i    j    k    l    m    n    o */
	1, 491, 85, 173, 232, 744, 127, 110, 293, 418,   6, 39, 250, 139, 429, 446,

	/* p    q    r    s    t    u    v    w    x    y    z    {    |    }    ~      */
	111,   5, 388, 375, 531, 152, 57, 97, 12, 101,   5,   2,   1,   2,   3,   1,

	1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
	1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
	1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
	1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
	1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
	1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
	1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
	1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
	1
};

//
char code[100000];
static int code_index = 0;
static int decode_index = 0;

//buffer
static int buffer;
//buffer
static int bits_to_go;
//
static int garbage_bits;
FILE *fp_in;
FILE *fp_encode;
FILE *fp_decode;
//
void start_model() {
	int i;
	for (i = 0; i<No_of_chars; i++) {
		//
		char_to_index[i] = i + 1;
		index_to_char[i + 1] = i;
	}

	//cum_freq[i-1]=freq[i]+...+freq[257], cum_freq[257]=0;
	cum_freq[No_of_symbols] = 0;
	for (i = No_of_symbols; i>0; i--) {
		cum_freq[i - 1] = cum_freq[i] + freq[i];
	}
	//
	//if (cum_freq[0] > Max_frequency);   /* Check counts within limit*/
}


//
void start_outputing_bits()
{
	buffer = 0;                                //
	bits_to_go = 8;
}


void output_bit(int bit)
{
	//！
	buffer >>= 1;
	if (bit) buffer |= 0x80;
	bits_to_go -= 1;
	//
	if (bits_to_go == 0) {
		code[code_index] = buffer;
		//
		//printf("%c ",code[code_index]);
		fwrite(&code[code_index], 1, 1, fp_encode);
		//fputc(code[code_index], fp_encode);
		code_index++;

		bits_to_go = 8; //
	}
}


void done_outputing_bits()
{
	//０
	char ch = buffer >> bits_to_go;
	code[code_index] = ch;
	//fputc(code[code_index], fp_encode);
	fwrite(&code[code_index], 1, 1, fp_encode);
	code_index++;
	//fputc(EOF, fp_encode);
	//fwrite(EOF, 1, 1, fp_encode);
}



static void bit_plus_follow(int);   /* Routine that follows                    */
static code_value low, high;    /* Ends of the current code region          */
static long bits_to_follow;     /* Number of opposite bits to output after */


void start_encoding()
{
	for (int i = 0; i<100; i++)code[i] = '\0';

	low = 0;                            /* Full code range.                 */
	high = Top_value;
	bits_to_follow = 0;                 /* No bits to follow           */
}


void encode_symbol(int symbol, int cum_freq[])
{
	long range;                 /* Size of the current code region          */
	range = (long)(high - low) + 1;

	high = low + (range*cum_freq[symbol - 1]) / cum_freq[0] - 1;  /* Narrow the code region  to that allotted to this */
	low = low + (range*cum_freq[symbol]) / cum_freq[0]; /* symbol.                  */

	for (;;)
	{                                  /* Loop to output bits.     */
		if (high<Half) {
			bit_plus_follow(0);                 /* Output 0 if in low half. */
		}
		else if (low >= Half) {                   /* Output 1 if in high half.*/
			bit_plus_follow(1);
			low -= Half;
			high -= Half;                       /* Subtract offset to top. */
		}
		else if (low >= First_qtr  && high<Third_qtr) {  /* Output an opposite bit　later if in middle half. */
			bits_to_follow += 1;
			low -= First_qtr;                   /* Subtract offset to middle*/
			high -= First_qtr;
		}
		else break;                             /* Otherwise exit loop.     */
		low = 2 * low;
		high = 2 * high + 1;                        /* Scale up code range.     */
	}
}

/* FINISH ENCODING THE STREAM. */

void done_encoding()
{
	bits_to_follow += 1;                       /* Output two bits that      */
	if (low<First_qtr) bit_plus_follow(0);     /* select the quarter that   */
	else bit_plus_follow(1);                   /* the current code range    */
}                                              /* contains.                 */


static void bit_plus_follow(int bit)
{
	output_bit(bit);                           /* Output the bit.           */
	while (bits_to_follow>0) {
		output_bit(!bit);                      /* Output bits_to_follow     */
		bits_to_follow -= 1;                   /* opposite bits. Set        */
	}                                          /* bits_to_follow to zero.   */
}



void encode() {
	start_model();                             /* Set up other modules.     */
	start_outputing_bits();
	start_encoding();
	for (;;) {                                 /* Loop through characters. */
		int ch;
		int symbol;
		ch = fgetc(fp_in);                     /* Read the next character. */
		if (ch == EOF)
			break;
		symbol = char_to_index[ch];            /* Translate to an index.    */
		encode_symbol(symbol, cum_freq);        /* Encode that symbol.       */

	}
	//EOF
	encode_symbol(EOF_symbol, cum_freq);
	done_encoding();                           /* Send the last few bits.   */
	done_outputing_bits();

}


//

static code_value value;        /* Currently-seen code value                */

void start_inputing_bits()
{
	bits_to_go = 0;                             /* Buffer starts out with   */
	garbage_bits = 0;                           /* no bits in it.           */
}


int input_bit()
{
	int t;
	//char char_in;

	if (bits_to_go == 0) {
		//buffer = fgetc(fp_encode); 
		fread(&buffer, 1, 1, fp_encode);
		//buffer = code[decode_index];
		//decode_index++;
		if (buffer == EOF) {
			//if(decode_index > code_index ){
			garbage_bits += 1;                      /* Return arbitrary bits*/
			if (garbage_bits>Code_value_bits - 2) {   /* after eof, but check */
				fprintf(stderr, "Bad input file/n"); /* for too many such.   */
				exit(-1);
			}
		}
		bits_to_go = 8;
	}
	//
	t = buffer & 1;                               /* Return the next bit from */
	buffer >>= 1;                               /* the bottom of the byte. */
	bits_to_go -= 1;
	return t;
}

void start_decoding()
{
	int i;
	value = 0;                                  /* Input bits to fill the   */
	for (i = 1; i <= Code_value_bits; i++) {      /* code value.              */
		value = 2 * value + input_bit();
	}


	low = 0;                                    /* Full code range.         */
	high = Top_value;
}


int decode_symbol(int cum_freq[])
{
	long range;                 /* Size of current code region              */
	int cum;                    /* Cumulative frequency calculated          */
	int symbol;                 /* Symbol decoded */
	range = (long)(high - low) + 1;
	cum = (((long)(value - low) + 1)*cum_freq[0] - 1) / range;    /* Find cum freq for value. */

	for (symbol = 1; cum_freq[symbol]>cum; symbol++); /* Then find symbol. */
	high = low + (range*cum_freq[symbol - 1]) / cum_freq[0] - 1;   /* Narrow the code region   *//* to that allotted to this */
	low = low + (range*cum_freq[symbol]) / cum_freq[0];

	for (;;) {                                  /* Loop to get rid of bits. */
		if (high<Half) {
			/* nothing */                       /* Expand low half.         */
		}
		else if (low >= Half) {                   /* Expand high half.        */
			value -= Half;
			low -= Half;                        /* Subtract offset to top. */
			high -= Half;
		}
		else if (low >= First_qtr && high <Third_qtr) {
			value -= First_qtr;
			low -= First_qtr;                   /* Subtract offset to middle*/
			high -= First_qtr;
		}
		else break;                             /* Otherwise exit loop.     */
		low = 2 * low;
		high = 2 * high + 1;                        /* Scale up code range.     */
		value = 2 * value + input_bit();            /* Move in next input blt. */
	}
	return symbol;
}

void decode() {
	start_model();                              /* Set up other modules.    */
	start_inputing_bits();
	start_decoding();
	for (;;) {                                  /* Loop through characters. */
		int ch; int symbol;
		symbol = decode_symbol(cum_freq);       /* Decode next symbol.      */
		if (symbol == EOF_symbol) break;          /* Exit loop if EOF symbol. */
		ch = index_to_char[symbol];             /* Translate to a character.*/
		fputc(ch, fp_decode);                        /* Write that character.    */
													 //putc(ch,stdout);                        /* Write that character.    */

	}
}

int check_filename(char * name)
{
	int len = strlen(name);
	if (len < 5 || name[len - 1] != 't' || name[len - 2] != 'x' || name[len - 3] != 't' || name[len - 4] != '.')
		return -1;
	else
		return len - 4;
}
int check_filename2(char * name)
{
	int len = strlen(name);
	if (len < 12) {
		return -1;
	}
	if (0 != strcmp(name + len - 11, "_encode.txt")) {
		return -1;
	}
	else
		return len - 11;
}
/*int main()
{
	

	////////
	scanf("%s", filename_encode);
	filename_len = check_filename2(filename_encode);
	filename_encode[filename_len] = '\0';
	filename_decode[0] = '\0';
	strcat(filename_decode, filename_encode);
	strcat(filename_decode, "_decode.txt");
	while (filename_len < 0 || (fp_encode = fopen(filename_encode, "r")) == NULL) {
		printf("open failed, try again!\n");
		scanf("%s", filename_encode);
		filename_len = check_filename2(filename_encode);
	}
	if (NULL == (fp_decode = fopen(filename_decode, "w"))) {
		printf("create failed!\n");
		exit(-1);
	}
	decode();
	fclose(fp_decode);
	fclose(fp_encode);

	return 0;
}*/
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Comp(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Decomp(HWND, UINT, WPARAM, LPARAM);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_NOUUU, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NOUUU));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

int ok11 = 0, ok12 = 0, ok21 = 0, ok22 = 0;


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NOUUU));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = CreateSolidBrush(RGB(217, 225, 226));//(HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_NOUUU);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		LPCTSTR WindowCaption = L"Windows Application";
		MessageBox(NULL, L"Bun-venit la aplicatia de compresie a datelor !", WindowCaption, MB_OK);
	}break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_FILE_COMPRESIE:
		{
			LPCTSTR WindowCaption = L"Compresia";
			MessageBox(NULL, L"Alege fisierul pentru compresia datelor...", WindowCaption, MB_OK);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX1), hWnd, Comp);
		}break;
		case ID_FILE_DECOMPRESIE:
		{
			LPCTSTR WindowCaption = L"Decompresia";
			MessageBox(NULL, L"Alege fisierul pentru decompresia datelor...", WindowCaption, MB_OK);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX2), hWnd, Decomp);
		}break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


// Message handler for about box.

INT_PTR CALLBACK Comp(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG: {
		HWND h = GetDlgItem(hDlg, IDC_LIST1);
		if (h == NULL) MessageBox(hDlg, L"EERR", L"DD", NULL);
		SendMessage(h, LB_SETHORIZONTALEXTENT, 500, NULL);
		return (INT_PTR)TRUE; }
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDC_BUTTON1:
		{
			ok11 = 1;
			/*OPENFILENAMEW ofn;       // common dialog box structure
			char szFile[260];       // buffer for file name
			HWND hwnd;              // owner window
			HANDLE hf;              // file handle

									// Initialize OPENFILENAME
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hDlg;
			ofn.lpstrFile = (LPTSTR)szFile;
			// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
			// use the contents of szFile to initialize itself.
			ofn.lpstrFile[0] = (WCHAR) '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = (LPCWSTR) "All\0*.*\0Text\0*.TXT\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			// Display the Open dialog box. 
			*/
			/*OPENFILENAME ofn;
			char szFileName[MAX_PATH] = "";
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
			ofn.hwndOwner = hDlg;
			ofn.lpstrFilter =(LPCWSTR) "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
			ofn.lpstrFile =(LPWSTR) szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			ofn.lpstrDefExt =(LPCWSTR) "txt";*/
			OPENFILENAME ofn;
			TCHAR szFile[MAX_PATH];
			
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.lpstrFile = szFile;
			ofn.lpstrFile[0] = '\0';
			ofn.hwndOwner = hDlg;
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = TEXT("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0");//TEXT("All files(*.*)\0*.*\0");
			ofn.nFilterIndex = 1;
			ofn.lpstrInitialDir = NULL;
			ofn.lpstrFileTitle = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			LPCTSTR WindowCaption = L"Ai ales fisierul cu numele...";

			
			
			//int index;
			//index = SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0, /*(LPARAM)"Hi there!"*/(LPARAM)szFile);

			if (GetOpenFileName(&ofn) == TRUE)
			{
				
				//index = SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0, /*(LPARAM)"Hi there!"*/(LPARAM)"Acesta este ...");
				//MessageBox(NULL, (LPCWSTR)szFile, WindowCaption, MB_ICONINFORMATION);
				wcstombs(fisintrare, szFile, wcslen(szFile) + 1);

				SetDlgItemText(hDlg, IDC_EDIT11, szFile);
				
				len = GetWindowTextLength(GetDlgItem(hDlg, IDC_EDIT11));
				if (len > 0)
				{
					int i;
					char* buf;
					TCHAR asa[MAX_PATH];
					buf = (char*)GlobalAlloc(GPTR, len + 1);
					GetDlgItemText(hDlg, IDC_EDIT11,asa, len + 1);
					//... do stuff with text ...
					GlobalFree((HANDLE)buf);
				}



				int index;
				LPCTSTR strMsg = L"A fost ales fisierul pentru intrare ...";
				LPCTSTR strMsg1 = L"Acesta este ...";
				index = SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0,(LPARAM)(LPCTSTR)strMsg);
				index = SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg1);
				index = SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0,(LPARAM)szFile);
			}

			/*char filename_in[1000];
			char filename_encode[1000];
			char filename_decode[1000];
			int filename_len = -1;

			strcpy(filename_in, fisintrare);
			filename_len = check_filename(filename_in);
			while (filename_len < 0 || (fp_in = fopen(filename_in, "r")) == NULL) {
				printf("open failed, try again!\n");
				scanf("%s", filename_in);
				filename_len = check_filename(filename_in);
			}
			filename_in[filename_len] = '\0';
			filename_encode[0] = '\0';
			strcat(filename_encode, filename_in);
			strcat(filename_encode, "_encode.txt");
			if (NULL == (fp_encode = fopen(filename_encode, "w"))) {
				printf("create failed!\n");
				exit(-1);
			}
			encode();
			fclose(fp_encode);
			fclose(fp_in);*/
			/*hf = CreateFile(ofn.lpstrFile,
			GENERIC_READ,
			0,
			(LPSECURITY_ATTRIBUTES)NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			(HANDLE)NULL);*/
			/*LPCTSTR WindowCaption = L"Alegere";
			MessageBox(NULL, L"Alege fisierul pentru care va avea loc compresia...", WindowCaption, MB_ICONINFORMATION);*/
			/*OPENFILENAME ofn;       // common dialog box structure
			char szFile[260];       // buffer for file name
			HWND hwnd;              // owner window
			HANDLE hf;              // file handle

			// Initialize OPENFILENAME
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFile =(LPWSTR) szFile;
			// Set lpstrFile[0] to '\0' so that GetOpenFileName does not
			// use the contents of szFile to initialize itself.
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter =(LPCWSTR) "All\0*.*\0Text\0*.TXT\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			// Display the Open dialog box.

			if (GetOpenFileName(&ofn))
			{
			MessageBox(NULL, (LPCWSTR)szFile, TEXT("You selected the file..."), MB_OK);
			//LPCTSTR WindowCaption = L"Ai ales fisierul...";
			//MessageBox(NULL,(LPCWSTR) szFileName, WindowCaption, MB_ICONINFORMATION);
			// Do something usefull with the filename stored in szFileName
			}
			*/
		}break;

		case IDC_BUTTON12:
		{
			ok12 = 1;
			OPENFILENAME ofn;
			TCHAR szFile[MAX_PATH];
			char fisiesire[MAX_PATH];
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.lpstrFile = szFile;
			ofn.lpstrFile[0] = '\0';
			ofn.hwndOwner = hDlg;
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = TEXT("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0");//TEXT("All files(*.*)\0*.*\0");
			ofn.nFilterIndex = 1;
			ofn.lpstrInitialDir = NULL;
			ofn.lpstrFileTitle = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			if (GetSaveFileName(&ofn) == TRUE)
			{
				wcstombs(fisiesire, szFile, wcslen(szFile) + 1);
				wcstombs(fisiesire1, szFile, wcslen(szFile) + 1);
				strcat(fisiesire, "_encode.txt");
				strcat(fisiesire1, "_encode.txt");
				//MultiByteToWideChar
				
				/*
				LPCTSTR strMs1 = L"Bla bla bla ...";
				TCHAR fullPath[MAX_PATH + 11]; // 11 = length of "\MyApp.exe" + nul in characters
				_sntprintf_s(fullPath, MAX_PATH + 11, _T("%s\\MyApp.exe"), strMs1);*/

				LPCTSTR strMs1 = L"Bla bla bla ...";
				TCHAR fullPath[MAX_PATH + 11]; // 11 = length of "\MyApp.exe" + nul in characters
				_sntprintf_s(fullPath, MAX_PATH + 11, _T("%s_encode.txt"), szFile);

				TCHAR fullPath1[MAX_PATH + 11];
				_sntprintf_s(fullPath1, MAX_PATH + 11, _T("%s.txt"), szFile);
				
				SetDlgItemText(hDlg, IDC_EDIT12, fullPath1);


				len1 = GetWindowTextLength(GetDlgItem(hDlg, IDC_EDIT12));
				if (len1 > 0)
				{
					int i;
					char* buf;
					TCHAR asa[MAX_PATH];
					buf = (char*)GlobalAlloc(GPTR, len1 + 1);
					GetDlgItemText(hDlg, IDC_EDIT12, asa, len1 + 1);
					//... do stuff with text ...
					GlobalFree((HANDLE)buf);
				}

				int index;
				LPCTSTR strMsg = L"A fost ales fisierul pentru iesire ...";
				LPCTSTR strMsg1 = L"Acesta este ...";
				index = SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg);
				index = SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg1);
				index = SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0, /*(LPARAM)"Hi there!"*/(LPARAM)fullPath1);
				//SetDlgItemText(hDlg, IDC_EDIT12, fullPath1);
				
				//Sleep(5000);
				LPCTSTR strMs2 = L"Fisierul dumneavoastra a fost redenumit pentru a fi gasit si observat mai usor ...";
				LPCTSTR strMsg3 = L"Noul sau nume este ...";

				LPCTSTR strMsg4 = L"Modificarea s-a efectuat si in campul edit pentru fisierul de iesire de mai sus ...";
				index = SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMs2);
				index = SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg3);
				index = SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0, /*(LPARAM)"Hi there!"*/(LPARAM)fullPath);

				SetDlgItemText(hDlg, IDC_EDIT12, fullPath);
				index = SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg4);
			}
			/*char filename_in[1000];
			char filename_encode[1000];
			char filename_decode[1000];
			int filename_len = -1;
			if (len != 0)
			{
				strcpy(filename_in, fisintrare);
				filename_len = check_filename(filename_in);
				while (filename_len < 0 || (fp_in = fopen(filename_in, "r")) == NULL) {
					printf("open failed, try again!\n");
					scanf("%s", filename_in);
					filename_len = check_filename(filename_in);
				}
				filename_in[filename_len] = '\0';
				filename_encode[0] = '\0';
				strcat(filename_encode, fisiesire);
				//strcat(filename_encode, "_encode.txt");
				if (NULL == (fp_encode = fopen(filename_encode, "w"))) {
					printf("create failed!\n");
					exit(-1);
				}
				encode();
				fclose(fp_encode);
				fclose(fp_in);
			}*/
		}break;
		//Aici trebuie pus codul pentru compresie
		case IDOK1:
		{
			int index;
			if(len==0 && len1==0)
			{
				LPCTSTR strMsg11 = L"Nu au fost selectate fisierul de intrare si fisierul de iesire !";
				index = SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg11);
			}
			else
				if (len == 0)
				{
					LPCTSTR strMsg11 = L"N-a fost selectat fisierul de intrare !";
					index = SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg11);
				}
				else
					if (len1 == 0)
					{
						LPCTSTR strMsg11 = L"N-a fost selectat fisierul de iesire !";
						index = SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg11);
					}
					else
						if (len != 0 && len1 != 0)
						{
							LPCTSTR strMsg = L"Sa inceapa compresia !";
							LPCTSTR strMsg1 = L"Compresia s-a finalizat !";
							index = SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg);
							index = SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg1);
							char filename_in[1000];
							char filename_encode[1000];
							char filename_decode[1000];
							int filename_len = -1;
							if (len != 0)
							{
								strcpy(filename_in, fisintrare);
								filename_len = check_filename(filename_in);
								while (filename_len < 0 || (fp_in = fopen(filename_in, "r")) == NULL) {
									printf("open failed, try again!\n");
									scanf("%s", filename_in);
									filename_len = check_filename(filename_in);
								}
								filename_in[filename_len] = '\0';
								filename_encode[0] = '\0';
								strcat(filename_encode, fisiesire1);
								//strcat(filename_encode, "_encode.txt");
								if (NULL == (fp_encode = fopen(filename_encode, "w"))) {
									printf("create failed!\n");
									exit(-1);
								}
								encode();
								fclose(fp_encode);
								fclose(fp_in);
							}
						}


			/*int index;
			if (ok11 == 0 && ok12 == 0)
			{
				LPCTSTR strMsg11 = L"Nu au fost selectate fisierul de intrare si fisierul de iesire !";
				index = SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg11);
			}
			//else
			//if (ok11 == 0 && ok12!=0)
			//{
				//LPCTSTR strMsg11 = L"N-a fost selectat fisierul de intrare !";
				//index = SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg11);
				//ok12 = 0;
			//}
			else
				if(ok12==0 && ok11!=0)
				{
					LPCTSTR strMsg11 = L"N-a fost selectat fisierul de iesire !";
					index = SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg11);
					ok11 = 0;
				}
				else
					if(len==0)
				{
					int index1;
					LPCTSTR strMsg5 = L"N-a fost introdus fisierul de iesire ...";
					index1 = SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg5);
				}
					else
					{
						LPCTSTR strMsg = L"Sa inceapa compresia !";
						LPCTSTR strMsg1 = L"Compresia s-a finalizat !";
						index = SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg);
						index = SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg1);
					}*/
			/*LPCTSTR WindowCaption = L"Compress...";
			MessageBox(NULL, L"Sa inceapa compresia!", WindowCaption, MB_ICONINFORMATION);*/
			
			/*LPCTSTR WindowCaption1 = L"Final...";
			MessageBox(NULL, L"Compresia s-a finalizat !", WindowCaption1, MB_ICONINFORMATION);*/

			/*LPCTSTR WindowCaption2 = L"Important !!!";
			MessageBox(NULL, L"Compresia s-a realizat in fisierul text < in_encode.txt > !", WindowCaption2, MB_ICONINFORMATION);*/
		}break;

		case IDC_EDIT11:
		{
			len = GetWindowTextLength(GetDlgItem(hDlg, IDC_EDIT11));
			if (len == 0)
			{
				int index;
				LPCTSTR strMsg1 = L"A-ti sters fisierul de intrare !";
				index = SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg1);
			}
		}break;

		case IDC_EDIT12:
		{
			len1 = GetWindowTextLength(GetDlgItem(hDlg, IDC_EDIT12));
			if (len1 == 0)
			{
				int index;
				LPCTSTR strMsg1 = L"A-ti sters fisierul de iesire !";
				index = SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg1);
			}
		}break;

		}
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}



INT_PTR CALLBACK Decomp(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		HWND h = GetDlgItem(hDlg, IDC_LIST2);
		if (h == NULL) MessageBox(hDlg, L"EERR", L"DD", NULL);
		SendMessage(h, LB_SETHORIZONTALEXTENT, 500, NULL);
		return (INT_PTR)TRUE;
	}
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
			case IDC_BUTTON2:
			{
				ok21 = 1;
				/*OPENFILENAMEW ofn;       // common dialog box structure
				char szFile[260];       // buffer for file name
				HWND hwnd;              // owner window
				HANDLE hf;              // file handle

										// Initialize OPENFILENAME
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hDlg;
				ofn.lpstrFile = (LPTSTR)szFile;
				// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
				// use the contents of szFile to initialize itself.
				ofn.lpstrFile[0] = (WCHAR) '\0';
				ofn.nMaxFile = sizeof(szFile);
				ofn.lpstrFilter = (LPCWSTR) "All\0*.*\0Text\0*.TXT\0";
				ofn.nFilterIndex = 1;
				ofn.lpstrFileTitle = NULL;
				ofn.nMaxFileTitle = 0;
				ofn.lpstrInitialDir = NULL;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;*/

				// Display the Open dialog box. 
				/*OPENFILENAME ofn;
				char szFileName[MAX_PATH] = "";
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
				ofn.hwndOwner = hDlg;
				ofn.lpstrFilter = (LPCWSTR)"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
				ofn.lpstrFile =(LPWSTR) szFileName;
				ofn.nMaxFile = MAX_PATH;
				ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
				ofn.lpstrDefExt = (LPCWSTR)"txt";*/
				OPENFILENAME ofn;
				TCHAR szFile[MAX_PATH];
				//char fisintrare[MAX_PATH];
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.lpstrFile = szFile;
				ofn.lpstrFile[0] = '\0';
				ofn.hwndOwner = hDlg;
				ofn.nMaxFile = sizeof(szFile);
				ofn.lpstrFilter = TEXT("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0");//TEXT("All files(*.*)\0*.*\0");
				ofn.nFilterIndex = 1;
				ofn.lpstrInitialDir = NULL;
				ofn.lpstrFileTitle = NULL;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
				LPCTSTR WindowCaption = L"Ai ales fisierul cu numele...";
				LPCTSTR WindowCaption1 = L"Am trecut de mesaj...";
				if (GetOpenFileName(&ofn) == TRUE)
				{
					//MessageBox(NULL, (LPCWSTR)szFile, WindowCaption, MB_ICONINFORMATION);
					wcstombs(fisintrare, szFile, wcslen(szFile) + 1);
					//SetDlgItemText(hDlg, IDC_EDIT1, szFile);
					//MessageBox(NULL, (LPCWSTR)szFile, WindowCaption1, MB_ICONINFORMATION);
					//int index = SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0, /*(LPARAM)"Hi there!"*/(LPARAM)szFile);

					SetDlgItemText(hDlg, IDC_EDIT1, szFile);

					len2 = GetWindowTextLength(GetDlgItem(hDlg, IDC_EDIT1));
					if (len2 > 0)
					{
						int i;
						char* buf;
						TCHAR asa[MAX_PATH];
						buf = (char*)GlobalAlloc(GPTR, len2 + 1);
						GetDlgItemText(hDlg, IDC_EDIT1, asa, len2 + 1);
						//... do stuff with text ...
						GlobalFree((HANDLE)buf);
					}


					int index;
					LPCTSTR strMsg = L"A fost ales fisierul pentru intrare ...";
					LPCTSTR strMsg1 = L"Acesta este ...";
					index = SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg);
					index = SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg1);
					index = SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, /*(LPARAM)"Hi there!"*/(LPARAM)szFile);

				}
				/*char filename_in[1000];
				char filename_encode[1000];
				char filename_decode[1000];
				int filename_len = -1;
				//scanf("%s", filename_encode);
				strcpy(filename_encode, fisintrare);
				filename_len = check_filename2(filename_encode);
				filename_encode[filename_len] = '\0';
				filename_decode[0] = '\0';
				strcat(filename_decode, filename_encode);
				strcat(filename_decode, "_decode.txt");
				strcpy(filename_encode, fisintrare);
				while (filename_len < 0 || (fp_encode = fopen(filename_encode, "r")) == NULL) {
					printf("open failed, try again!\n");
					scanf("%s", filename_encode);
					filename_len = check_filename2(filename_encode);
				}
				if (NULL == (fp_decode = fopen(filename_decode, "w"))) {
					printf("create failed!\n");
					exit(-1);
				}
				decode();
				fclose(fp_decode);
				fclose(fp_encode);*/
			}break;

			case IDC_BUTTON22:
			{
				ok22 = 1;
				OPENFILENAME ofn;
				TCHAR szFile[MAX_PATH];
				char fisiesire[MAX_PATH];
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.lpstrFile = szFile;
				ofn.lpstrFile[0] = '\0';
				ofn.hwndOwner = hDlg;
				ofn.nMaxFile = sizeof(szFile);
				ofn.lpstrFilter = TEXT("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0");//TEXT("All files(*.*)\0*.*\0");
				ofn.nFilterIndex = 1;
				ofn.lpstrInitialDir = NULL;
				ofn.lpstrFileTitle = NULL;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
				if (GetSaveFileName(&ofn) == TRUE)
				{
					wcstombs(fisiesire, szFile, wcslen(szFile) + 1);
					wcstombs(fisiesire2, szFile, wcslen(szFile) + 1);
					strcat(fisiesire, "_decode.txt");
					strcat(fisiesire2, "_decode.txt");
					/*SetDlgItemText(hDlg, IDC_EDIT2, szFile);
					int index;
					LPCTSTR strMsg = L"A fost ales fisierul pentru iesire ...";
					LPCTSTR strMsg1 = L"Acesta este ...";
					index = SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg);
					index = SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg1);
					index = SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LPARAM)szFile);*/


					//
					TCHAR fullPath[MAX_PATH + 11]; // 11 = length of "\MyApp.exe" + nul in characters
					_sntprintf_s(fullPath, MAX_PATH + 11, _T("%s_decode.txt"), szFile);

					TCHAR fullPath1[MAX_PATH + 11];
					_sntprintf_s(fullPath1, MAX_PATH + 11, _T("%s.txt"), szFile);

					SetDlgItemText(hDlg, IDC_EDIT2, fullPath1);

					len3 = GetWindowTextLength(GetDlgItem(hDlg, IDC_EDIT2));
					if (len3 > 0)
					{
						int i;
						char* buf;
						TCHAR asa[MAX_PATH];
						buf = (char*)GlobalAlloc(GPTR, len3 + 1);
						GetDlgItemText(hDlg, IDC_EDIT11, asa, len3 + 1);
						//... do stuff with text ...
						GlobalFree((HANDLE)buf);
					}


					int index;
					LPCTSTR strMsg = L"A fost ales fisierul pentru iesire ...";
					LPCTSTR strMsg1 = L"Acesta este ...";
					index = SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg);
					index = SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg1);
					index = SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, /*(LPARAM)"Hi there!"*/(LPARAM)fullPath1);
					//SetDlgItemText(hDlg, IDC_EDIT12, fullPath1);

					//Sleep(5000);
					LPCTSTR strMs2 = L"Fisierul dumneavoastra a fost redenumit pentru a fi gasit si observat mai usor ...";
					LPCTSTR strMsg3 = L"Noul sau nume este ...";

					LPCTSTR strMsg4 = L"Modificarea s-a efectuat si in campul edit pentru fisierul de iesire de mai sus ...";
					index = SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMs2);
					index = SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg3);
					index = SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, /*(LPARAM)"Hi there!"*/(LPARAM)fullPath);

					SetDlgItemText(hDlg, IDC_EDIT2, fullPath);
					index = SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg4);

				}
				/*char filename_in[1000];
				char filename_encode[1000];
				char filename_decode[1000];
				int filename_len = -1;
				strcpy(filename_encode, fisintrare);
				filename_len = check_filename2(filename_encode);
				filename_encode[filename_len] = '\0';
				filename_decode[0] = '\0';
				strcat(filename_decode, fisiesire);
				//strcat(filename_decode, "_decode.txt");
				strcpy(filename_encode, fisintrare);
				while (filename_len < 0 || (fp_encode = fopen(filename_encode, "r")) == NULL) {
					printf("open failed, try again!\n");
					scanf("%s", filename_encode);
					filename_len = check_filename2(filename_encode);
				}
				if (NULL == (fp_decode = fopen(filename_decode, "w"))) {
					printf("create failed!\n");
					exit(-1);
				}
				decode();
				fclose(fp_decode);
				fclose(fp_encode);*/
			}break;
			//Aici trebuie pus codul pentru decompresie
			case IDOK2:
			{
				int index;
				if (len2 == 0 && len3 == 0)
				{
					LPCTSTR strMsg11 = L"Nu au fost selectate fisierul de intrare si fisierul de iesire !";
					index = SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg11);
				}
				else
					if (len2 == 0)
					{
						LPCTSTR strMsg11 = L"N-a fost selectat fisierul de intrare !";
						index = SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg11);
					}
					else
						if (len3 == 0)
						{
							LPCTSTR strMsg11 = L"N-a fost selectat fisierul de iesire !";
							index = SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg11);
						}
						else
							if (len2 != 0 && len3 != 0)
							{
								LPCTSTR strMsg = L"Sa inceapa decompresia !";
								LPCTSTR strMsg1 = L"Decompresia s-a finalizat !";
								index = SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg);
								index = SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg1);
								char filename_in[1000];
								char filename_encode[1000];
								char filename_decode[1000];
								int filename_len = -1;
								strcpy(filename_encode, fisintrare);
								filename_len = check_filename2(filename_encode);
								filename_encode[filename_len] = '\0';
								filename_decode[0] = '\0';
								strcat(filename_decode, fisiesire2);
								//strcat(filename_decode, "_decode.txt");
								strcpy(filename_encode, fisintrare);
								while (filename_len < 0 || (fp_encode = fopen(filename_encode, "r")) == NULL) {
									printf("open failed, try again!\n");
									scanf("%s", filename_encode);
									filename_len = check_filename2(filename_encode);
								}
								if (NULL == (fp_decode = fopen(filename_decode, "w"))) {
									printf("create failed!\n");
									exit(-1);
								}
								decode();
								fclose(fp_decode);
								fclose(fp_encode);
								
							}

				/*int index;
				if (ok21 == 0 && ok22 == 0)
				{
					LPCTSTR strMsg11 = L"Nu au fost selectate fisierul de intrare si fisierul de iesire !";
					index = SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg11);
				}
				else
					if (ok21 == 0 && ok22 != 0)
					{
						LPCTSTR strMsg11 = L"N-a fost selectat fisierul de intrare !";
						index = SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg11);
					}
					else
						if (ok22 == 0 && ok21 != 0)
						{
							LPCTSTR strMsg11 = L"N-a fost selectat fisierul de iesire !";
							index = SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg11);
						}
						else
						{
							LPCTSTR strMsg = L"Sa inceapa decompresia !";
							LPCTSTR strMsg1 = L"Decompresia s-a finalizat !";
							index = SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg);
							index = SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg1);
						}*/

				/*int index;
				LPCTSTR strMsg = L"Sa inceapa decompresia !";
				LPCTSTR strMsg1 = L"Decompresia s-a finalizat !";
				index = SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg);
				index = SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg1);*/
				/*LPCTSTR WindowCaption = L"Decompress...";
				MessageBox(NULL, L"Sa inceapa decompresia!", WindowCaption, MB_ICONINFORMATION);*/

				/*LPCTSTR WindowCaption1 = L"Final...";
				MessageBox(NULL, L"Decompresia s-a finalizat !", WindowCaption1, MB_ICONINFORMATION);*/

				/*LPCTSTR WindowCaption2 = L"Important !!!";
				MessageBox(NULL, L"Decompresia s-a realizat in fisierul text < in_decode.txt > !", WindowCaption2, MB_ICONINFORMATION);*/
			}break;

			case IDC_EDIT1:
			{
				len2 = GetWindowTextLength(GetDlgItem(hDlg, IDC_EDIT1));
				if (len2 == 0)
				{
					int index;
					LPCTSTR strMsg1 = L"A-ti sters fisierul de intrare !";
					index = SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg1);
				}
			}break;

			case IDC_EDIT2:
			{
				len3 = GetWindowTextLength(GetDlgItem(hDlg, IDC_EDIT2));
				if (len3 == 0)
				{
					int index;
					LPCTSTR strMsg1 = L"A-ti sters fisierul de iesire !";
					index = SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strMsg1);
				}
			}break;




		}
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	}
	return (INT_PTR)FALSE;
}




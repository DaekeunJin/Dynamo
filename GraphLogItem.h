#pragma once
#include <windows.h>

class CGraphLogSetting {
public:
	BOOL use{ FALSE };
	int item_type{ 0 };
	int data1_type{ 0 };  // 0:Const,   1:Data  2:Math
	int iData1_val{ 0 };
	double fData1_val{ 0. };
	int data2_type{ 0 };  // 0:Const,   1:Data  2:Math
	int iData2_val{ 0 };
	double fData2_val{ 0. };
	int cond{ 0 };	 // 0: >    1: >=    2: ==   3: !=    4: <    5: <=
	int range[4] { 0, 0, 0, 0};    
	COLORREF color1{ 0 };
	COLORREF color2{ 0 };
	BOOL bAvailable{ FALSE };
	BOOL use_mask{ FALSE };
	int mask_val{ 0 };
	int digit{ 0 };
    int div{ 0 };
	bool bUpdateStr{ false };
};


class CDrawLogData {
public:
	BOOL use{ FALSE };
	int type{ 0 };
	CString strText{""};
	CPoint ptText{ 0, 0 };
	int range[4]{ 0, };
	COLORREF color1{ 0 };
	COLORREF color2{ 0 };
};


class CTextLogSetting {
public:
	BOOL use{ FALSE };
	short item_type{ 0 };
	short data_type{ 0 };  // 0:Data  1:Math
	short data_id{ 0 };
	unsigned int mask_val{ 0 };
	unsigned int ref_iVal{ 0 };
	double ref_fVal{ 0, };
	BOOL show1{ FALSE };
	BOOL show2{ FALSE };
	CString strComment1{ "" };
	CString strComment2{ "" };
    short dp{ 0 };
    short divide{ 0 };
};




//
//	文字表示
//

#ifndef	__PRINTF_
#define	__PRINTF_

//--- 定数 ----------------------------------------------------------------------------------------
#define	PRINTF_CHARA_MAX	2000					// 最大文字数


//--- 関数宣言 ------------------------------------------------------------------------------------
													// 文字色を設定
extern	void	printf_front_color_set(uchar a, uchar r, uchar g, uchar b);
													// 文字の背景色を設定
extern	void	printf_back_color_set(uchar a, uchar r, uchar g, uchar b);
													// 文字リセット
extern	void	printf_reset(void);
													// 文字初期化
extern	void	printf_init(void);
													// 文字描画
extern	void	printf_flush(void);
													// ２Ｄ座標で文字登録
extern	void	printf_2d(int iX, int iY, int iSize, BOOL bBack, char *Format,...);
extern	void	printf_2d(int iX, int iY, int iSize,			 char *Format,...);
extern	void	printf_2d(int iX, int iY,						 char *Format,...);
extern	void	printf_2d(										 char *Format,...);

#ifdef	__CALC
														// ３Ｄ座標で文字登録
	extern	void	printf_3d(VEC3 *pvecPos, int iSize, BOOL bBack, char *Format,...);
	extern	void	printf_3d(VEC3 *pvecPos, int iSize,             char *Format,...);
	extern	void	printf_3d(VEC3 *pvecPos,                        char *Format,...);
#endif


//--- 旧版対応用
#define	Printf		printf_2d
#define	PrintfInit	printf_init
#define	PrintfFlush	printf_flush


#endif			//	__PRINTF_



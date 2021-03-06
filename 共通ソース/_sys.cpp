//
//	システムもろもろ
//

//--- 関数宣言 ------------------------------------------------------------------------------------
extern	void	ConfigSave(char	*str);
extern	void	ConfigLoad(char	*str);
extern	void	ConfigDefault(void);
extern	bool	EndMessage(void);
static	void	EscToExit(void);
											// マウスがウィンドウの描画矩形内か調べる
#define	RECT_CHECK()	{ if(d3.rectRender != NULL && (pad.sX >= d3.sRenderW ||	pad.sY >= d3.sRenderH))	break;}

#define	COMPILE_MULTIMON_STUBS				// Multimon.h 用のスイッチ(削除してはいけない
#include	<Multimon.h>					// マルチモニター(sys.h で定義すると他と競合してエラーがでる場合あり

//--- ライブラリィ --------------------------------------------------------------------------------
//#pragma comment(lib, "shlwapi.lib")
//#pragma comment(lib, "odbc32.lib")
//#pragma comment(lib, "odbccp32.lib")
#pragma comment(lib, "winmm.lib")
//#pragma comment(lib, "imm32.lib")
#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "shlwapi.lib")
//#pragma comment(lib, "user32.lib")

//odbc32.lib odbccp32.lib d3d9.lib d3dx9.lib winmm.lib dsound.lib dxguid.lib strmiids.lib dinput8.lib wsock32.lib d3dxof.lib imm32.lib Comctl32.lib

static	void	FixWindowPos(void);


//--- 各種プログラム ------------------------------------------------------------------------------

#ifdef	__D3
	#include	"system\\_d3.cpp"
	#include	"system\\_d3_mesh.cpp"
	#include	"system\\_shader.cpp"
#else
	D3_WORK		d3;
#endif

#ifdef	__PAD
	#include	"system\\_pad.cpp"
#endif
#ifdef	__SND
	#include	"system\\_snd.cpp"
#endif
#ifdef	__SKIN
	#include	"system\\_skin.cpp"
#endif
#ifdef	__NET
	#include	"ex\\_net.cpp"
#endif
#ifdef	__CAMERA
	#include	"ex\\_camera.cpp"
#endif
#ifdef	__MQO
	#include	"ex\\_mqo.cpp"
#endif
#ifdef	__MOVIE
	#include	"ex\\_movie_to_texture.cpp"
#endif
#ifdef	__PRINTF
	#include	"ex\\_printf.cpp"
#endif
#ifdef	__IME
	#include	"ex\\_ime.cpp"
#endif
#ifdef	__TABLET
	#include	"ex\\_tablet.cpp"
#endif
#ifdef	__CALC
	#include	"ex\\_calc.cpp"
#endif
#ifdef	__COLLISION
	#include	"ex\\_collision.cpp"
#endif
#ifdef	__MESH_BUF
	#include	"ex\\_mesh_buf.cpp"
#endif
#ifdef	__ASORT
	#include	"ex\\_asort.cpp"
#endif
#ifdef	__FONT_TO_TEXTURE
	#include	"ex\\_font_to_texture.cpp"
#endif
#ifdef	__FRUSTUM
	#include	"ex\\_frustum.cpp"
#endif
#ifdef	__SHADER_PASTEL
	#include	"ex\\_pastel.cpp"			// ヘッダにダミーあり
#endif
#ifdef	__SHADER_NORMAL_EDGE
	#include	"ex\\_edge.cpp"				// ヘッダにダミーあり
#endif
#ifdef	__SHADER_GLARE
	#include	"ex\\_glare.cpp"			// ヘッダにダミーあり
#endif


//--- 調整中 --------------------------------------------------------
#ifdef	__SCRIPT
	#include	"ex\\_script.cpp"
	#include	"ex\\_menu.cpp"
#endif
#ifdef	__AFONT
	#include	"ex\\_afont.cpp"
#endif
//-------------------------------------------------------------------

// コントロールは標準搭載
#include	"ctrl\\_ctrl.cpp"

// 画面設定ダイアログ
#ifdef	IDM_DEVICE_CHANGE
	#include	"ctrl\\_ctrl_d3.cpp"
#endif
#ifdef	__VERSION
	#include	"ctrl\\_ctrl_version_dialog.cpp"
#endif

#include	"ctrl\\_ctrl_debug.cpp"


//--- ダミー --------------------------------------------------------------------------------------
#ifndef	__PAD
	PAD_WORK	pad;
	inline	bool	pad_init(void){return true;}
	inline	void	pad_main(void)
	{
		// マウスだけは DirectInput で行わない
		pad.sMoveX = pad.sX	- pad.sXBak;
		pad.sMoveY = pad.sY	- pad.sYBak;
		pad.sMoveZ = pad.sZ	- pad.sZBak;
		pad.sXBak  = pad.sX;
		pad.sYBak  = pad.sY;
		pad.sZBak  = pad.sZ;

		pad.bLeftClick      = false;
		pad.bRightClick     = false;
		pad.bCenterClick    = false;
		pad.bLeftRelease    = false;
		pad.bRightRelease   = false;
		pad.bCenterRelease  = false;
		pad.bLeftDblClick	= false;
		pad.bRightDblClick	= false;
		pad.bCenterDblClick	= false;
	}
	inline	bool	pad_end(void){return true;}
	inline	void	pad_pause(bool bSw){}
	inline	void	pad_check(void){}
#endif

#ifndef	__SND
	inline	bool	snd_init(void){return true;}
	inline	void	snd_main(void){}
	inline	void	snd_end(void){}
	inline	void	snd_pause_all(bool){}
	inline	void	snd_system_pause_all(bool){}
	inline	bool	snd_load(int iNum, char *strOriFile, bool bLoop){return true;}
	inline	void	snd_play(int iNum){}
	inline	void	snd_play_delay(int iNum, int iDelay){}
	inline	void	snd_stop(int iNum){}
	inline	void	snd_free(int iNum){}
	inline	void	snd_volume(int iNum, float fVol, int iTime){}
	inline	void	snd_pitch( int iNum, float fVol, int iTime){}
#endif

#ifdef	__TSK
	#include	"ex\\_tsk.cpp"
	#include	"ex\\_tsk_move.cpp"
#else
	inline	bool	tsk_init(void){return true;}
	inline	void	tsk_free(void){}
#endif

#ifndef	__SKIN
	inline	void	skin_free(int iNum){return;}
	inline	bool	skin_load(int iNum, char *strFile, bool bLoadAtOnce, bool bBumpLoad, bool bFurLoad){return false;}
	MTX				mtxSkinPalette[1];			// ダミー
#endif


//--- 関数宣言 ----------------------------------------------------------------
											// この関数の実態は main.cpp に置くこと
static	bool	ExEvent(HWND hw, UINT iMsg, WPARAM wParam, LPARAM lParam);



/*------------------------------------------------------------------------------*
| <<< システムの一時休止設定 >>>
*------------------------------------------------------------------------------*/
static	uchar	ucPauseCount;				// ポーズＯＮのときの遅延時間(これで、すぐに始まらないように抑制している)
void	sys_pause(void)
{
	// デバイス未作成時は相手にしない
	if(d3.dev == NULL){ return;}

	// 強制でない限り、bAlwayExec はここで終了
	//if(!bCompulsion && sys.bAlwayExec){ return;}

	// ポーズを掛ける
	if(ucPauseCount == 0)// && !sys.bPause)
	{
		//ucPauseCount = 0;
		snd_system_pause_all(true);
		#ifdef	__MOVIE
			movie_to_texture_system_pause(true);
		#endif
		SYS_WAIT_RESET();
		sys.bPause   = true;
		ucPauseCount = 10;					// 10 はすぐにＯＮにならないよう抑制する時間です。10 は適当
	}
//	// ポーズＯＦＦ
//	else
//	{
//		ucPauseCount = 10;					// 10 はすぐにＯＮにならないよう抑制する時間です。10 は適当
//	}
	// フラグは保存しておく
	//sys.bPause = bSw;
}



/*------------------------------------------------------------------------------*
| <<< ポーズメイン処理 >>>	※ここでは主にポーズの解除を行う
*------------------------------------------------------------------------------*/
static	void	PauseMain(void)
{
	if(sys.bAlwayExec){ return;}

	// 解除はすぐに行わないように時間を計測する
	if(ucPauseCount > 0)
	{
		ucPauseCount--;
		if(ucPauseCount == 0)
		{
			sys.bPause = false;
			snd_system_pause_all(sys.bPause);
			#ifdef	__MOVIE
				movie_to_texture_system_pause(sys.bPause);
			#endif
			SYS_WAIT_RESET();
		}
	}
}



/*------------------------------------------------------------------------------*
| <<< ウィンドウがプライマリーにあるか判定する >>>
*------------------------------------------------------------------------------*/
void	PrimaryCheck(void)
{
	#if	WINVER >= 0x0500
		RECT	r, rectWindow;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &r, 0);	// プライマリーのウィンドウサイズを得る
		GetWindowRect(sys.hWnd, &rectWindow);
		if(rectWindow.left   > r.right 
		|| rectWindow.right  < r.left  
		|| rectWindow.top    > r.bottom
		|| rectWindow.bottom < r.top)
		{
			sys.bPrimary = false;
		}
		else
		{
			sys.bPrimary = true;
		}
	#else
		sys.bPrimary = true;
	#endif
}



/*------------------------------------------------------------------------------*
| <<< ＭＰ３イベント >>>
*------------------------------------------------------------------------------*/
static	void	SndEvent(HWND hw, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	#ifdef	__SND
		uint	a = iMsg - MP3_MESSAGE0;
		if(a < SND_MAX) snd_mp3_proc(a);
	#endif
}



/*------------------------------------------------------------------------------*
| <<< マウスイベント >>>
*------------------------------------------------------------------------------*/
static	void	MouseEvent(HWND	hw,	UINT iMsg, WPARAM wParam, LPARAM lParam)
{

	switch(iMsg)
	{
	// ここでホイールをチェックすると
	// たのコントロールにフォーカスがあたった状態の時に動作しなくなる
	//case WM_MOUSEWHEEL:
	//	pad.sZ += ((short)HIWORD(wParam)) /	120;
	//	break;
	//case WM_MOUSEMOVE:
		/*if(d3.rectRender !=	NULL)
		{
			pad.sX = (short)(LOWORD(lParam)	- d3.rectRender->left);
			pad.sY = (short)(HIWORD(lParam)	- d3.rectRender->top);
			break;
		}
		else

		#ifdef	__2D_RESIZE
			pad.sXOri = (short)LOWORD(lParam);
			pad.sYOri = (short)HIWORD(lParam);
			pad.sX    = (short)(((float)pad.sXOri - d3.vecOfs.x) / d3.fZoom);
			pad.sY    = (short)(((float)pad.sYOri - d3.vecOfs.y) / d3.fZoom);
		#else
			pad.sXOri = (short)LOWORD(lParam);
			pad.sYOri = (short)HIWORD(lParam);
			pad.sX    = pad.sXOri;
			pad.sY    = pad.sYOri;
			#ifdef	__RENDER_FIXATION
				if(d3.bWindow)
				{
					pad.sX = (short)(pad.sX / sys.fXZoom);
					pad.sY = (short)(pad.sY / sys.fYZoom);
				}
				else
				{
					pad.sX = (short)(((float)(pad.sX) * (float)__SCR_W) / (float)d3.sFullScreenRenderW);
					pad.sY = (short)(((float)(pad.sY) * (float)__SCR_H) / (float)d3.sFullScreenRenderH);
				}
			#endif
		#endif
		break;
		*/
	case WM_LBUTTONDBLCLK:
		RECT_CHECK();
		pad.bLeftDblClick = true;
		break;

	case WM_RBUTTONDBLCLK:
		RECT_CHECK();
		pad.bRightDblClick = true;
		break;

	case WM_MBUTTONDBLCLK:
		RECT_CHECK();
		pad.bCenterDblClick = true;
		break;

	case WM_LBUTTONDOWN:
		if(d3.rectRender !=	NULL &&	(pad.sX	>= d3.sRenderW || pad.sY >=	d3.sRenderH)) break;
		pad.bLeftPush  = true;
		pad.bLeftClick = true;
		SetCapture(sys.hWnd);						// ウィンドウ外でもマウスの値が拾えるようにキャプチャー
		break;

	case WM_RBUTTONDOWN:
		RECT_CHECK();
		pad.bRightPush  = true;
		pad.bRightClick = true;
		SetCapture(sys.hWnd);						// ウィンドウ外でもマウスの値が拾えるようにキャプチャー
		break;

	case WM_MBUTTONDOWN:
		RECT_CHECK();
		pad.bCenterPush  = true;
		pad.bCenterClick = true;
		SetCapture(sys.hWnd);						// ウィンドウ外でもマウスの値が拾えるようにキャプチャー
		break;

	case WM_LBUTTONUP:
		pad.bLeftPush    = false;
		pad.bLeftRelease = true;
		ReleaseCapture();							// キャプチャーしたマウスを開放
		break;

	case WM_RBUTTONUP:
		pad.bRightPush    = false;
		pad.bRightRelease = true;
		ReleaseCapture();							// キャプチャーしたマウスを開放
		break;

	case WM_MBUTTONUP:
		pad.bCenterPush    = false;
		pad.bCenterRelease = true;
		ReleaseCapture();							// キャプチャーしたマウスを開放
		break;
	}
}



/*------------------------------------------------------------------------------*
| <<< ウィンドウプロシージャ >>>
*------------------------------------------------------------------------------*/
LRESULT	CALLBACK	procWin(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
											// 拡張イベント(game_event)
	if(ExEvent(hWnd, uiMsg, wParam, lParam)) return 0;
	MouseEvent(hWnd, uiMsg, wParam, lParam);	// マウスイベント
	SndEvent(  hWnd, uiMsg, wParam, lParam);	// MP3 イベント


	#ifdef	__TABLET
		//--- タブレットのイベント呼び出し ------------------------------
		TabletProc(hWnd, uiMsg, wParam, lParam);
		//---------------------------------------------------------------
	#endif

	switch(uiMsg)
	{
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
			#ifdef	IDM_DEVICE_CHANGE
				case IDM_DEVICE_CHANGE:								// デバイス変更
					#ifdef	__D3
						sys_pause();
						if(!d3.bWindow)
						{
							d3.bWindow = true;
							d3_screen_change(true);
						}
						//extern	void	ctrl_d3_open(void);
						ctrl_d3_open();
					#endif
			#endif
					break;

			#ifdef	IDM_FULL_WINDOW_CHANGE
				case IDM_FULL_WINDOW_CHANGE:						// ウィンドウ⇔フルスクリーン
					while(sys.bMain);
					d3.bWindow ^= 1;
					sys_pause();
					sys.cResizeCount  =	0;
					sys.bScreenChange =	true;
					break;
			#endif
			#ifdef IDM_WINDOW_RESET
				case IDM_WINDOW_RESET:								// ウィンドウリセット
					{
						int	iW = GetSystemMetrics(SM_CXSCREEN) / 2 - __SCR_W / 2;
						int	iH = GetSystemMetrics(SM_CYSCREEN) / 2 - __SCR_H / 2;
						SetRect(&sys.rectWindow, iW, iH, iW + __SCR_W + SYS_XFRAME_GET(), iH + __SCR_H + SYS_YFRAME_GET());
						SetRect(&sys.rectWindow, iW, iH, iW + __SCR_W, iH + __SCR_H);
						AdjustWindowRect(&sys.rectWindow, WINDOW_STYLE1, true);

						sys.rectWindowRestore = sys.rectWindow;
						sys.cResizeCount	  =	0;
						sys.bScreenChange	  =	true;
						sys.cWindowStyle	  = SIZE_RESTORED;
						sys.fXZoom			  = 1.0f;
						sys.fYZoom			  = 1.0f;
						break;
					}
			#endif
		case IDM_END:										// 終了
			if(SYS_EXIT_MENU() && EndMessage()){ SendMessage(sys.hWnd, WM_DESTROY, 0, 0);}
			break;

			#ifdef	IDM_GRAPICHS_RELOAD
				case IDM_GRAPICHS_RELOAD:							// 画像、３Ｄモデル再読み込み
					d3_reload_set();
					break;
			#endif

			#ifdef	IDM_ZENMEN
				case IDM_ZENMEN:									// 常に前面表示
					MENU_CHECK(IDM_ZENMEN, sys.bTopMost	= 1, sys.bTopMost =	0);
					MENU_SET(  IDM_ZENMEN, sys.bTopMost	== 1);
					sys_window_topmost_set();
					break;
			#endif
		}
		break;
	case WM_TIMER:							// ファイル監視のためのタイマーイベント
		if(wParam == ID_MYTIMER){ sys_file_watch_main();}
		break;
	case WM_IME_SETCONTEXT:					// ＩＭＥ関係
	case WM_IME_STARTCOMPOSITION:
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_IME_NOTIFY:
		#if	IME_PROC
			ime_proc(sys.hWnd, iMsg, wParam, lParam);
			return 0;
		#else
			return 0;
		#endif

		#ifdef	__NET
			case WM_WINSOCKMESSAGE:					// winsockメッセージ
				return net_proc(hWnd, uiMsg, wParam, lParam);
		#endif
	case WM_SIZE:
		{
			while(sys.bMain);							// メインループ待ち(これやらないと、ウィンドウサイズ変更中にDirectXの描画が入るのでフリーズしやすくなります)
														// これで、フルスクリーン⇔ウィンドウの切り替え時の割り込みを防ぐ
			if(sys.cResizeCount	== DEVICE_MAKE_TIME){ break;}
			sys_pause();								// 割り込み等を停止
			if(wParam == SIZE_MINIMIZED)				// 最小化時
			{
			}
			if(wParam == SIZE_MAXIMIZED)				// 最大化時
			{
				sys.rectWindowRestore = sys.rectWindow;
				GetWindowRect(sys.hWnd, &sys.rectWindow);
				sys.cResizeCount = 0;					// デバイス再作成要求
			}
			if(wParam == SIZE_RESTORED)
			{
				sys.rectWindow   = sys.rectWindowRestore;
				sys.cResizeCount = 0;					// デバイス再作成要求
			}
														// ウィンドウのスタイルを保存しておく
			if(!sys.bStartUp){ sys.cWindowStyle = wParam;}

		#ifndef	__RENDER_FIXATION
			if(d3.bWindow)
			{
				sys.fXZoom = (sys.rectWindow.right  - sys.rectWindow.left - SYS_XFRAME_GET()) / (float)__SCR_W;
				sys.fYZoom = (sys.rectWindow.bottom - sys.rectWindow.top  - SYS_YFRAME_GET()) / (float)__SCR_H;
			}
			else
			{
				sys.fXZoom = sys.fYZoom = 1;
			}
		#else
			sys.fXZoom = sys.fYZoom = 1;
		#endif
			UpdateWindow(sys.hWnd);
			break;
		}
	case WM_DEVICECHANGE:					// ジョイスティックの常態が変わった
		pad_check();
		break;
	//case WM_MOVING:
	//case WM_MOVE:							// WM_MOVE         は最少化の時にも呼ばれる
	case WM_EXITSIZEMOVE:					// WM_EXITSIZEMOVE は最小化、最大化の時には呼ばれない
		sys_pause();
		PrimaryCheck();						// ウィンドウがプライマリーにあるか判定する
		GetWindowRect(sys.hWnd, &sys.rectWindow);
		//sys.rectWindowRestore = sys.rectWindow;
		break;
	case WM_PAINT:
		if(d3.dev != NULL
		&& sys.cWindowStyle != SIZE_MINIMIZED
		&& sys.cResizeCount	== DEVICE_MAKE_TIME	+ 1
		&& d3.bPaintPresent)
		{
			#ifdef	__D3
				d3_present();
			#endif
		}
		break;
	case WM_POWERBROADCAST:					// 休止、スリープ
#if 0
		while(sys.bMain);
		if(wParam == PBT_APMRESUMESUSPEND)
		{
			int	i;		
			for(i =	D3_SHADER_TYPE_NORMAL; i < D3_SHADER_TYPE_MAX; i++)
			{
				if(d3.lpEffect[i - D3_SHADER_TYPE_NORMAL]){ d3.lpEffect[i - D3_SHADER_TYPE_NORMAL]->OnResetDevice();}
			}
			// ここ、vistaでエラーがでる可能性有り
			for(i =	0; i < D3_FONT_MAX; i++)
			{
				if(d3.font[i]){ d3.font[i]->OnLostDevice();}
			}
			d3.dev->Reset(&d3.para);
			if(sys.bScreenChange){ WindowStyleSet(d3.bWindow);}
			else	  { UpdateWindow(sys.hWnd);}

			d3_view_port_set();
			d3_stat_filter_set(D3_FILTER_LINEAR);
			d3_shader_set(     D3_SHADER_TYPE_OFF);

			d3.dev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);	// この一行が無いと面が白くなります。
			d3.dev->SetRenderState(D3DRS_AMBIENT,		   D3_AMBIENT);
			d3_stat_dither_set(true);

			d3_offscreen_repair();							// オフスクリーン情報修復

			//--- 画面をクリアー --------------------------------------------
			d3_render_begin(/*D3_COL(255, 236, 233, 216)*/);	// レンダー開始
			d3_render_end(false);								// レンダリング終了
			bSuspend = FALSE;
			//sys.cResizeCount	= DEVICE_MAKE_TIME;
		}
		else if(wParam == PBT_APMSUSPEND)
		{
			bSuspend = TRUE;
			//d3_offscreen_save();
			//d3_offscreen_free();
			//sys.cResizeCount = 0;
		}
#endif
		break;

	case WM_DISPLAYCHANGE:					// ディスプレイ変更イベント
		if(d3.bDeviceChange){ break;}		// このアプリで解像度変更中に呼び出された場合はここで終了
		while(sys.bMain);
		FixWindowPos();						// 表示表示を再計算
		#ifdef	__D3
			if(!d3.bWindow){ d3.bWindow	= true; d3_screen_change(true);}
		#endif
			break;
	case WM_SYSCOMMAND:
		sys_pause();
		if(!d3.bWindow)
		{
			d3.bWindowBak = d3.bWindow;
			d3.bWindow	= true;
			d3_screen_change(true);
		}
		switch(wParam)
		{
		case SC_MOVE:
		case SC_SIZE:
		case SC_MAXIMIZE:
		case SC_KEYMENU:
		case SC_MONITORPOWER:
			//while(sys.bMain);
			//if(!d3.bWindow){ d3.bWindow	= true; d3_screen_change(true);}
			//return DefWindowProc(hWnd, uiMsg, wParam, lParam);
			break;
		}
		break;
	case WM_CLOSE:							// ウィンドウ左上の×ボタンを押した時
		if(SYS_EXIT_MENU() && EndMessage())
		{
			SendMessage(sys.hWnd, WM_DESTROY, 0, 0);
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uiMsg, wParam, lParam);
}



/*------------------------------------------------------------------------------*
| <<< メイン処理 >>>
*------------------------------------------------------------------------------*/
static	void	AppMain(void)
{

	//--- マウスの座標計算 ------------------------------------------
	// WM_MOUSEMOVE で行うと、ウィンドウの範囲外がとれないのでここで行う
	POINT	p;
	GetCursorPos(			 &p);
	ScreenToClient(sys.hWnd, &p);

	if(d3.rectRender !=	NULL)
	{
		pad.sX = (short)(p.x - d3.rectRender->left);
		pad.sY = (short)(p.y - d3.rectRender->top);
	}
	else
	{
		#ifdef	__2D_RESIZE
			pad.sXOri = (short)p.x;
			pad.sYOri = (short)p.y;
			pad.sX    = (short)(((float)pad.sXOri - d3.vecOfs.x) / d3.fZoom);
			pad.sY    = (short)(((float)pad.sYOri - d3.vecOfs.y) / d3.fZoom);
		#else
			pad.sXOri = (short)p.x;
			pad.sYOri = (short)p.y;
			pad.sX    = pad.sXOri;
			pad.sY    = pad.sYOri;
			#ifdef	__RENDER_FIXATION
				if(d3.bWindow)
				{
					pad.sX = (short)(pad.sX / sys.fXZoom);
					pad.sY = (short)(pad.sY / sys.fYZoom);
				}
				else
				{
					pad.sX = (short)(((float)(pad.sX) * (float)__SCR_W) / (float)d3.sFullScreenRenderW);
					pad.sY = (short)(((float)(pad.sY) * (float)__SCR_H) / (float)d3.sFullScreenRenderH);
				}
			#endif
		#endif
	}

	//--- デバイス未作成時、最小時 ----------------------------------
#ifdef	__D3
	if(!d3.d ||	!d3.dev	|| sys.cWindowStyle	== SIZE_MINIMIZED)
	{
	}
	else
#endif
	{
		#ifdef	__D3
			//--- デバイス再作成、再作成待ち ----------------------------
			if(sys.cResizeCount < DEVICE_MAKE_TIME + 1)
			{
				sys.cResizeCount++;
				if(sys.cResizeCount	== DEVICE_MAKE_TIME)
				{
					sys_window_size_set();		// sys.iWidth, sys.iHeight を設定
					if(sys.funcResize != NULL)	// リサイズは登録時のみ呼び出される
					{
						void(*func)(void);
						func = (void(*)(void))sys.funcResize;
						(*func)();
					}
					d3_screen_change(sys.bScreenChange);
					sys_window_size_set();		// sys.iWidth, sys.iHeight を設定
					sys.bScreenChange =	false;
					sys.cResizeCount++;
					d3.bRender = true;
 					PrimaryCheck();				// ウィンドウがプライマリーにあるか判定する
				}
			}
			else
		#endif
			//--- 通常の処理 --------------------------------------------
			if((!sys.bPause || sys.bTopMost || sys.bAlwayExec) && !d3.bDeviceChange)
			{
				sys.bMain =	true;
				#ifdef	__GAME_RENDER
					game_main();
					d3_render();
				#else
					#ifdef	__D3
						d3_render_sub(game_main);
					#else
						game_main();
					#endif
				#endif
				sys.bMain =	false;
			}
	}
	EscToExit();
}



/*------------------------------------------------------------------------------*
| <<< スクリーンセーバー禁止、コールバック関数 >>>
*------------------------------------------------------------------------------*/
static	BOOL	CALLBACK	StopScreenSaverProc(HWND hWnd, LPARAM lParam)
{
	PostMessage(hWnd, WM_CLOSE,	0, 0L);
	SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, TRUE,	0, SPIF_SENDWININICHANGE);
	return	TRUE;
}



/*------------------------------------------------------------------------------*
| <<< スクリーンセーバー禁止 >>>
*------------------------------------------------------------------------------*/
static	void	StopScreenSaver(void)
{

	OSVERSIONINFO	osInfo;

	osInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osInfo);

	if(osInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
	{										// Windows95
		HWND	hWnd = FindWindow("WindowsScreenSaverClass", NULL);
		if(hWnd	!= NULL){ PostMessage(hWnd, WM_CLOSE, 0, 0L);}
	}
	else if(osInfo.dwPlatformId	== VER_PLATFORM_WIN32_NT)
	{										// WindowsNT
		HDESK	hDesk =	OpenDesktop("Screen-saver",	0, FALSE, DESKTOP_READOBJECTS |	DESKTOP_WRITEOBJECTS);
		if(hDesk)
		{
			EnumDesktopWindows(hDesk, StopScreenSaverProc, 0);
			CloseDesktop(hDesk);
		}
	}
}



/*------------------------------------------------------------------------------*
| <<< タイムスタンプをみてファイルを再読み込み >>>
|	※game_init で sys_file_watch_init を呼び出すと
|	  メッシュテクスチャー、シェーダーが更新されたときに自動で再読み込みする
*------------------------------------------------------------------------------*/
void	sys_file_watch_main(void)
{

	#ifdef	__D3
		int	i;

		//--- メッシュ監視 ----------------------------------------------
		for(i = 0; i < D3_MESH_MAX; i++)					// テクスチャーは一括で管理しているので、
		{													// ここではチェックしていない。
			if(d3.meshInfo[i].strFullPath[0] != NULL)		// ただし、メッシュが更新されている場合は、テクスチャーも再度読み込まれる
			{
				if(sys_timestamp_get(&d3.meshInfo[i].ftLastWrite, d3.meshInfo[i].strFullPath))
				{
					char	str[MAX_PATH + 1];
					strcpy(str, d3.meshInfo[i].strFullPath);
					if(d3.meshInfo[i].bSkin)
					{
						skin_free(i);
						skin_load(i, str, true, false, false);
					}
					else
					{
						d3_xfile_free(i);
						d3_xfile_load(i, str, true);
					}
					sys.ucFileWatchReload = 2;
					sys.usFileWatchNum    = i;
					return;
				}
			}
		}
		//--- テクスチャー監視 ------------------------------------------
		for(i = 0; i < D3_TEX_MAX2; i++)
		{												// ↓すでにロードしているもののみチェックする
			if(d3.texInfo[i].strFullPath[0] != NULL && d3.texInfo[i].bLoad)
			{											// テクスチャーも監視する
				if(sys_timestamp_get(&d3.texInfo[i].ftLastWrite, d3.texInfo[i].strFullPath))
				{
					char	str[MAX_PATH + 1];
					strcpy(str, d3.texInfo[i].strFullPath);
					d3_tex_free(i);
					d3_tex_load(i, str);
					sys.ucFileWatchReload = 1;
					sys.usFileWatchNum    = i;
					return;
				}
			}
		}

		//--- シェーダー監視 --------------------------------------------
		for(i = 0; i < D3_SHADER_TYPE_MAX; i++)
		{												// ↓すでにロードしているもののみチェックする
			if(d3.shaderInfo[i].strFullPath[0] != NULL)// && d3.shaderInfo[i].bLoad)
			{											// テクスチャーも監視する
				if(sys_timestamp_get(&d3.shaderInfo[i].ftLastWrite, d3.shaderInfo[i].strFullPath))
				{
					d3_shader_free(i);
					d3_shader_compile(i);
					d3.bRender = true;
				}
			}
		}
	#endif
}



/*------------------------------------------------------------------------------*
| <<< タイマーによりファイル監視の初期化 >>>
|	※game_init で sys_file_watch_init を呼び出すと
|	  メッシュテクスチャー、シェーダーが更新されたときに自動で再読み込みする
*------------------------------------------------------------------------------*/
void	sys_file_watch_init(void)
{											// タイマーイベント発動
	SetTimer(sys.hWnd, ID_MYTIMER, TIMER_COUNT, NULL);
}



/*------------------------------------------------------------------------------*
| <<< タイマーによりファイル監視の終了 >>>
*------------------------------------------------------------------------------*/
void	sys_file_watch_free(void)
{											// タイマーイベント終了
	KillTimer(sys.hWnd, ID_MYTIMER);
}



/*
DLGPROC	OfnOrgTopDlgProc;

UINT	CALLBACK	OfnTopDlgProc(HWND hwnd, UINT uiMsg, WPARAM wp, LPARAM lp)
{

	char	buff[128];

	switch(uiMsg)
	{
	case WM_SIZE:
		{
			RECT	r;
			GetClientRect(hwnd, &r);// ウィンドウのサイズを得る

			CTRL_WINDOW_RECT_SET(hwnd, r.right - r.left, r.bottom - r.top
				+ GetSystemMetrics(SM_CYSIZEFRAME) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYSIZEFRAME));
		}
		wsprintf(buff, "WM_SIZE (%d, %d)\n", LOWORD(lp), HIWORD(lp));
		OutputDebugString(buff);
		break;
	}
	return	CallWindowProc((WNDPROC)OfnOrgTopDlgProc, hwnd, uiMsg, wp, lp);
}



//--------------------------------------------------------------------------
UINT	CALLBACK OfnHookDlgProc(HWND hwnd, UINT wm, WPARAM wp, LPARAM lp)
{

	static	HWND	hTopDlg;

	switch(wm)
	{
	case WM_INITDIALOG:
		{
			OfnOrgTopDlgProc = (DLGPROC)SetWindowLong(GetParent(hwnd), DWL_DLGPROC, (DWORD)OfnTopDlgProc);
		}
		break;
	case WM_DESTROY:
		if(!OfnOrgTopDlgProc) SetWindowLong(hTopDlg, DWL_DLGPROC, (DWORD)OfnOrgTopDlgProc);
		break;
	}
	return FALSE;
}
*/



/*------------------------------------------------------------------------------*
| <<< ファイルの選択 >>>
|	入力	sw = 0 : SAVE
|				 1 : LOAD
|			*s = キャプション
|			*f = フィルター
|	戻り値	true  :	選択した
|			false :	未選択
|	出力	sys.strSelFile = 選択ファイル名
*------------------------------------------------------------------------------*/
bool	sys_file_select(int	sw,	char *s, char *f)
{
	OPENFILENAME	ofn;

	ZeroMemory(&ofn, sizeof(ofn));

	// OPENFILENAME	構造体の初期化
	ofn.lStructSize		  =	sizeof(ofn);
	ofn.hwndOwner		  =	sys.hWnd;
	ofn.lpstrFile		  =	sys.strSelFile;	
	ofn.nMaxFile		  =	MAX_PATH;
	ofn.lpstrFilter		  =	f;
	ofn.nFilterIndex	  =	1;
	ofn.lpstrTitle		  =	s;
	ofn.lpstrInitialDir	  =	NULL;
	ofn.lpstrCustomFilter =	NULL;
	ofn.nMaxCustFilter	  =	0;
	ofn.lpstrFileTitle	  =	NULL;
	ofn.nMaxFileTitle	  =	0;
	ofn.nFileOffset		  =	0;
	ofn.nFileExtension	  =	0;
	ofn.lpstrDefExt		  =	NULL;
	ofn.lCustData		  =	0;
	ofn.lpfnHook		  =	NULL;//OfnHookDlgProc;	// フックを掛けると旧スタイルになるので、辞めた
	ofn.lpTemplateName	  =	0;																								// ↓複数ファイル選択時は設定する
	ofn.Flags			  =	OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | /*OFN_ENABLEHOOK | */OFN_ENABLESIZING;// | OFN_ALLOWMULTISELECT;
	if(sw == 0)
	{
		if(GetSaveFileName(&ofn) ==	false){ return false;}
	}
	else
	{
		if(GetOpenFileName(&ofn) ==	false){ return false;}
	}
	return true;
}



/*------------------------------------------------------------------------------*
| <<< エラーメッセージの表示 >>>
|	入力	*func =	関数名
|			*mes  =	エラーメッセージ
|	戻り値	false :	必ずこの値を返す
*------------------------------------------------------------------------------*/
bool	sys_error(char*	Format,...)
{

	vsprintf(sys.strErr, Format, (char *)(&Format + 1));

	if(sys.funcError != NULL)				// エラー時に登録関数を呼び出す
	{
		void(*func)(char *str);
		func = (void(*)(char *str))sys.funcError;
		(*func)(sys.strErr);
	}
	else
	{
		#ifdef	__ERROR_MES_OUTPUT
			char	str[2048];
			sprintf(str, "★%s\n", sys.strErr);
			OutputDebugString(str);
		#else
			sys_message_box(sys.strErr, "エラー発生", MB_OK | MB_SYSTEMMODAL | MB_ICONEXCLAMATION);
		#endif
	}
	return false;
}



/*------------------------------------------------------------------------------*
| <<< システムメッセージの表示 >>>
|	入力	*func =	関数名
|			*mes  =	エラーメッセージ
|	戻り値	false :	必ずこの値を返す
*------------------------------------------------------------------------------*/
void	sys_printf(char* Format,...)
{

	vsprintf(sys.strErr, Format, (char *)(&Format + 1));

	if(sys.funcError != NULL)				// エラー時に登録関数を呼び出す
	{
		void(*func)(char *str);
		func = (void(*)(char *str))sys.funcError;
		(*func)(sys.strErr);
	}
	else
	{
		char	str[512];
		wsprintf(str, "%s\n", sys.strErr);
		OutputDebugString(str);
	}
}



/*------------------------------------------------------------------------------*
| <<< メッセージボックスフック処理 >>>
*------------------------------------------------------------------------------*/
static	HHOOK	hookMessageHandle;			// フックハンドル変数

LRESULT	CALLBACK	ProcMessageHook(int iCode, WPARAM wParam, LPARAM lParam)
{
	if(iCode == HCBT_ACTIVATE)
	{
		RECT	r;
		GetClientRect((HWND)wParam, &r);// ウィンドウのサイズを得る
										// ウィンドウを親のウィンドウの中心へ
		CTRL_WINDOW_RECT_SET((HWND)wParam, r.right  - r.left + GetSystemMetrics(SM_CXFIXEDFRAME) * 2,
										   r.bottom - r.top  + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION));
										// フック関数をアンインストール(フック解除）
		UnhookWindowsHookEx(hookMessageHandle);
	}
	// 次のフックへ
	return CallNextHookEx(hookMessageHandle, iCode, wParam, lParam);
}



/*------------------------------------------------------------------------------*
| <<< メッセージボックスフック処理 >>>
*------------------------------------------------------------------------------*/
int	sys_message_box(char *pstrMes, char *pstrCaption, UINT uiType)
{

	// フックを掛ける(最小化の時はかけない)
	if(sys.cWindowStyle	!= SIZE_MINIMIZED){ hookMessageHandle = SetWindowsHookEx(WH_CBT, ProcMessageHook, NULL, GetCurrentThreadId());}

	char	str[256];
	sprintf(str, "%s    ", pstrMes);
	return  MessageBox(sys.hWnd, str, pstrCaption, uiType);
}



/*------------------------------------------------------------------------------*
| <<< ESC キーで EXIT >>>
*------------------------------------------------------------------------------*/
static	void	EscToExit(void)
{
	#ifdef	__ESC_TO_EXIT

		if(sys.iEscSkip	> 0){ sys.iEscSkip--; return;}

		#ifdef	__PAD
			if(pad.bKeyClick[KEY_ESC])
		#else
		if(sys.bActive && 
			GetAsyncKeyState(VK_ESCAPE) & 0x8000)	// DirectInput を使わない場合は直接、キーを見る
		#endif
			{
				#ifdef	__D3
					// フルスクリーン時はいったんウィンドウモードへ以降
					sys_pause();
					if(!d3.bWindow)
					{
						d3.bWindowBak = d3.bWindow; d3.bWindow	= true;	d3_screen_change(true); return;}
				#endif

				if(!SYS_EXIT_MENU()){ return;}
				#ifdef	__PAD
					pad.bKeyClick[KEY_ESC] = false;
				#endif
				while(sys.bMain);						// メインループ処理待ち
				SendMessage(sys.hWnd, WM_DESTROY, 0, 0L);
			}
	#endif
}



//--- ウィンドウのキャプション --------------------------------------------------------------------

/*------------------------------------------------------------------------------*
| <<< ウィンドウのキャプション設定 >>>
|	入力	printf と同じです
*------------------------------------------------------------------------------*/
void	sys_caption_printf(char *Format, ...)
{
	char	buf[512];

	vsprintf(buf, Format, (char	*)(&Format + 1));
	SetWindowText(sys.hWnd,	buf);
}



//--- メニュー ------------------------------------------------------------------------------------

/*------------------------------------------------------------------------------*
| <<< フルスクリーンのメニュー表示管理 >>>
|	入力	bSw = true  : 表示オン
|				= false : 表示オフ
*------------------------------------------------------------------------------*/
void	sys_menu_view_set(bool bSw)
{
	if(bSw)									// 表示オン
	{
		if(!sys.bMenu)
		{
			sys.bMenu = true;
			SetMenu(sys.hWnd, sys.hMenu);
		}
	}
	else									// 表示オフ
	{
		if(sys.bMenu) sys.bMenu = false, SetMenu(sys.hWnd,	NULL);
	}
}



/*------------------------------------------------------------------------------*
| <<< メニューにチェックをつけたり外したり >>>
|	入力	idm = メニュー ID
|			sw  = true  : チェックをつける
|				= false : チェックを外す
*------------------------------------------------------------------------------*/
void	sys_menu_set(uint uiIDM, bool bSw)
{
	if(bSw){ CheckMenuItem(GetMenu(sys.hWnd), uiIDM, MF_BYCOMMAND | MF_CHECKED  );}	
	else{    CheckMenuItem(GetMenu(sys.hWnd), uiIDM, MF_BYCOMMAND | MF_UNCHECKED);}
}



/*------------------------------------------------------------------------------*
| <<< メニューを無効にする >>>
|	入力	idm = メニュー ID
*------------------------------------------------------------------------------*/
void	sys_menu_disable(uint uiIDM)
{
	MENUITEMINFO	info;
	ZeroMemory(&info, sizeof(MENUITEMINFO)); 
	info.cbSize = sizeof(info);
	info.fMask  = MIIM_STATE;
	info.fState = MFS_DISABLED;
	SetMenuItemInfo(sys.hMenu, uiIDM, FALSE, &info);
}



/*------------------------------------------------------------------------------*
| <<< メニューを有効にする >>>
|	入力	idm = メニュー ID
*------------------------------------------------------------------------------*/
void	sys_menu_enable(uint uiIDM)
{
	MENUITEMINFO	info;
	ZeroMemory(&info, sizeof(MENUITEMINFO)); 
	info.cbSize = sizeof(info);
	info.fMask  = MIIM_STATE;
	info.fState = MFS_ENABLED;
	SetMenuItemInfo(sys.hMenu, uiIDM, FALSE, &info);
}



/*------------------------------------------------------------------------------*
| <<< メニューの文字列を変更する >>>
|	入力	idm = メニュー ID
|			str = 変更文字列
*------------------------------------------------------------------------------*/
void	sys_menu_str_chage(uint uiIDM, char *str)
{
	MENUITEMINFO	info;
	ZeroMemory(&info, sizeof(MENUITEMINFO)); 
	info.cbSize     = sizeof(info);
	info.fMask      = MIIM_TYPE;
	info.fType      = MFT_STRING;
	info.wID		= uiIDM;
	info.dwTypeData = (LPTSTR)str;
	SetMenuItemInfo(sys.hMenu, uiIDM, FALSE, &info);
}



/*------------------------------------------------------------------------------*
| <<< メニューにセパレーターを追加 >>>
*------------------------------------------------------------------------------*/
void	sys_menu_separator_add(void)
{
	InsertMenu(GetSubMenu(GetMenu(sys.hWnd), 0), __SEPARATOR, MF_BYPOSITION | MFT_STRING | MFT_SEPARATOR, 0, NULL);
}



/*------------------------------------------------------------------------------*
| <<< メニューに最近使ったファイルを追加 >>>
*------------------------------------------------------------------------------*/
void	sys_menu_file_make(void)
{											// メニューに過去のファイルを追加
	int	i;
	for(i =	0; i < FILE_BAK; i++){ DeleteMenu(GetSubMenu(GetMenu(sys.hWnd), 0), 10000 + i, MF_BYCOMMAND);}
	for(i =	0; i < FILE_BAK; i++)
	{
		if(strlen(sys.strFileBak[i]) > 0)
		{
			InsertMenu(GetSubMenu(GetMenu(sys.hWnd), 0), __SEPARATOR + 1 + i, MF_BYPOSITION | MFT_STRING, 10000 + i, sys.strFileBak[i]);
		}
	}
}



/*------------------------------------------------------------------------------*
| <<< メニューに最近使ったファイルを追加 >>>
|	入力	__file = ファイル名
|			b      = true  : ファイルを追加
|				   = false : 同項目を削除
*------------------------------------------------------------------------------*/
bool	sys_menu_file_add(char *__file,	bool b)
{
	//--- 追加の場合 ------------------------------------------------
	if(b)
	{
		int		i;
											// まずは同名ファイルを探す
		for(i =	FILE_BAK - 1; i	!= -1; i--){ if(strcmp(sys.strFileBak[i], __file) == 0)	break;}
		if(i !=	-1)							// 同名ファイルがある場合は先頭にファイル名を持ってくる
		{
			for(int	j =	i; j !=	0; j--){	strcpy(sys.strFileBak[j], sys.strFileBak[j - 1]);}
			strcpy(sys.strFileBak[0], __file);
		}
		else								// 同名ファイルがない場合
		{
			for(i =	FILE_BAK - 1; i	!= 0; i--){ strcpy(sys.strFileBak[i], sys.strFileBak[i - 1]);}
			strcpy(sys.strFileBak[i], __file);
		}
		MENU_FILE_MAKE();
		return true;
	}
	//--- 同項目を削除 ----------------------------------------------
	else
	{										// 同じファイルがなければなにもしない
		int		i;
		for(i =	FILE_BAK - 1; i	!= -1; i--)
		{
			if(strcmp(sys.strFileBak[i], __file) ==	0){ break;}
		}
		if(i !=	-1)
		{
			for(; i	< FILE_BAK - 1;	i++){ strcpy(sys.strFileBak[i], sys.strFileBak[i + 1]);}
			sys.strFileBak[i][0] = NULL;
			MENU_FILE_MAKE();
		}
		return false;
	}
}



//--- 半角大文字を半角小文字へ --------------------------------------------------------------------

/*------------------------------------------------------------------------------*
| <<< strlwr と同じ >>>
|	入力	src = 大文字を小文字へ
|	出力	src = 変換文字列
*------------------------------------------------------------------------------*/
void	sys_strlwr(char *src)
{
	uint	a = (uint)strlen(src);
	for(uint i = 0; i < a; i++)
	{
		if(ISKANJI(src[i]))
		{
			i++;
		}
		else
		{
			if(src[i] >= 'A' && src[i]	<= 'Z') src[i] += 0x20;
		}
	}
}
	


//--- メッセージボックス --------------------------------------------------------------------------

/*------------------------------------------------------------------------------*
| <<< 終了メニュー >>>
|	戻り値	true or false
*------------------------------------------------------------------------------*/
bool	sys_exit_menu(void)
{
	#ifdef	__EXIT_MENU
		if(!d3.bWindow){ d3.bWindowBak = d3.bWindow; d3.bWindow	= true;	d3_screen_change(true);}

		sys_pause();

		#ifdef	__EXIT_MESSAGE
			int	id = sys_message_box(__EXIT_MESSAGE,   "確認", MB_YESNO | MB_ICONQUESTION | MB_SYSTEMMODAL);
		#else
			int	id = sys_message_box("終了しますか？", "確認", MB_YESNO | MB_ICONQUESTION | MB_SYSTEMMODAL);
		#endif
			
		sys_pause();
		if(id == IDYES){ return true;}
		else		   { return false;}
	#else
		return	true;
	#endif
}



/*------------------------------------------------------------------------------*
| <<< はい、いいえを問う >>>
|	入力	str = メッセージ
|	戻り値	true or false
*------------------------------------------------------------------------------*/
bool	sys_yes_no(char	*str)
{
	int	id = sys_message_box(str, "確認", MB_YESNO | MB_ICONQUESTION);
	if(id == IDYES){ return true;}
	else		   { return false;}
}



/*------------------------------------------------------------------------------*
| <<< はい、いいえ、キャンセルを問う >>>
|	入力	str = メッセージ
|	戻り値	true or false
*------------------------------------------------------------------------------*/
int		sys_yes_no_change(char *str)
{
	return sys_message_box(str, "確認", MB_YESNOCANCEL | MB_ICONQUESTION);
}



//--- デスクトップのサイズを返す ------------------------------------------------------------------

/*------------------------------------------------------------------------------*
| <<< タスクバーを除くサイズですデスクトップのＸサイズを得る >>>
*------------------------------------------------------------------------------*/
int		sys_desktop_xsize(void)
{
	#if	WINVER >= 0x0500
		return	GetSystemMetrics(SM_CXVIRTUALSCREEN);
	#else
		return	GetSystemMetrics(SM_CXSCREEN);
	#endif
}



/*------------------------------------------------------------------------------*
| <<< タスクバーを除くサイズですデスクトップのＹサイズを得る >>>
*------------------------------------------------------------------------------*/
int		sys_desktop_ysize(void)
{
	#if	WINVER >= 0x0500
		return	GetSystemMetrics(SM_CYVIRTUALSCREEN);
	#else
		return	GetSystemMetrics(SM_CYSCREEN);
	#endif
}



//--- ウィンドウ処理 ------------------------------------------------------------------------------

/*------------------------------------------------------------------------------*
| <<< フレームのＸサイズを得る >>>		※メインウィンドウ限定
*------------------------------------------------------------------------------*/
int		sys_xframe_get(void)
{
	if(sys_window_style_get() & WS_THICKFRAME){ return GetSystemMetrics(SM_CXSIZEFRAME ) * 2;}
	else									  { return GetSystemMetrics(SM_CXFIXEDFRAME) * 2;}
}



/*------------------------------------------------------------------------------*
| <<< フレームのＸサイズを得る >>>
|	入力	hWnd = ウィンドウハンドル
*------------------------------------------------------------------------------*/
int		sys_xframe_get(HWND hWnd)
{
	RECT	rc0, rc1;								// ウィンドウの縁のサイズを得る
	GetWindowRect(hWnd,	&rc0);
	GetClientRect(hWnd,	&rc1);
	return (rc0.right  - rc0.left) - (rc1.right	 - rc1.left);
}



/*------------------------------------------------------------------------------*
| <<< フレームのＹサイズを得る >>>		※メインウィンドウ限定
*------------------------------------------------------------------------------*/
int		sys_yframe_get(void)
{
	if(sys_window_style_get() & WS_THICKFRAME)
	{
		// サイズ変更ができるウィンドウの場合
		return GetSystemMetrics(SM_CYSIZEFRAME ) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYMENU) + GetSystemMetrics(SM_CYSIZEFRAME );
	}
	else
	{
		// サイズ変更ができないウィンドウの場合
		return GetSystemMetrics(SM_CYFIXEDFRAME) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYMENU) + GetSystemMetrics(SM_CYFIXEDFRAME);
	}
}



/*------------------------------------------------------------------------------*
| <<< フレームのＹサイズを得る >>>
|	入力	hWnd = ウィンドウハンドル
*------------------------------------------------------------------------------*/
int		sys_yframe_get(HWND hWnd)
{
	RECT	rc0, rc1;								// ウィンドウの縁のサイズを得る
	GetWindowRect(hWnd,	&rc0);
	GetClientRect(hWnd,	&rc1);
	return (rc0.bottom - rc0.top) - (rc1.bottom - rc1.top);
}



/*------------------------------------------------------------------------------*
| <<< フレームのトップより、エッジ、メニュー、キャプションを引いた値を得る >>>
*------------------------------------------------------------------------------*/
int		sys_topframe_get(void)
{													// フレームのサイズとは違うので注意
	if(sys_window_style_get() & WS_THICKFRAME){ return GetSystemMetrics(SM_CYSIZEFRAME ) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYMENU);}
	else									  { return GetSystemMetrics(SM_CYFIXEDFRAME) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYMENU);}
}



/*------------------------------------------------------------------------------*
| <<< ウィンドウサイズを設定する >>>
*------------------------------------------------------------------------------*/
void	sys_window_size_set(void)
{
	if(d3.bWindow)
	{
		sys.iWidth	= (sys.rectWindow.right	 - sys.rectWindow.left - SYS_XFRAME_GET());
		sys.iHeight	= (sys.rectWindow.bottom - sys.rectWindow.top  - SYS_YFRAME_GET());

		#ifdef	__RENDER_FIXATION
			if(d3.rectRender == NULL)
			{
				d3.sRenderW	= sys.iWidth;
				d3.sRenderH	= sys.iHeight;
			}
			else
			{
				d3.sRenderW	= (short)(d3.rectRender->right  - d3.rectRender->left);
				d3.sRenderH	= (short)(d3.rectRender->bottom - d3.rectRender->top );
			}
		#else
			d3.sRenderW	= __RENDER_X1 - __RENDER_X0;
			d3.sRenderH	= __RENDER_Y1 - __RENDER_Y0;
		#endif
	}
	else
	{
		RECT	r;
		GetWindowRect(sys.hWnd,	&r);
		if(d3.bWindow)
		{
			sys.iWidth	= (r.right	- r.left);
			sys.iHeight	= (r.bottom	- r.top);
		}
		else
		{
			sys.iWidth  = __RENDER_X1 - __RENDER_X0;
			sys.iHeight = __RENDER_Y1 - __RENDER_Y0;
		}
		#ifdef	__RENDER_FIXATION
			d3.sRenderW	= d3.sFullScreenRenderW;
			d3.sRenderH	= d3.sFullScreenRenderH;
		#else
			d3.sRenderW	= __SCR_W;
			d3.sRenderH	= __SCR_H;
		#endif
	}
}



//--- フォルダー操作 ------------------------------------------------------------------------------

/*----------------------------------------------------------------------*
| <<< ファイル名をパスとファイル名に分割する >>>
|	※	拡張子が無いとフォルダー名がファイル扱いになるので、
|		拡張子がない場合は、末尾に\\を足してください。										
|	入力	*strFile		= フルパスのファイル名
|	出力	*strPathOut		= パス
|			*strFileNameOut	= ファイル名
|			*strExtOut		= 拡張子名
*----------------------------------------------------------------------*/
void	sys_div_path(char *strPathOut, char	*strFileNameOut, char *strExtOut, char *strFile)
{
	char	file[ _MAX_PATH	 + 1];
	char	drive[_MAX_DRIVE + 1];
	char	dir[  _MAX_DIR	 + 1];
	char	name[ _MAX_FNAME + 1];
	char	ext[  _MAX_EXT	 + 1];

	if(strFile[1] != ':')					// フルパスでない場合はフォルダー位置を得る
	{
		char	strDir[_MAX_PATH + 1];
		_getcwd(strDir,	MAX_PATH);
		sprintf(file, "%s\\%s",	strDir,	strFile);
	}
	else
	{
		strcpy(file, strFile);
	}
	_splitpath(file, drive,	dir, name, ext);
	if(strPathOut	  != NULL){ _makepath(strPathOut,	 drive,	dir,  NULL,	NULL);}
	if(strExtOut	  != NULL)				// 拡張子を要求しているか？
	{
		_makepath(strExtOut, NULL, NULL, NULL, ext);
		if(strFileNameOut != NULL){ _makepath(strFileNameOut, NULL, NULL, name, NULL);}
	}
	else									// 拡張子を要求していない場合は
	{										// ファイル名 +	拡張子とする
		if(strFileNameOut != NULL){ _makepath(strFileNameOut, NULL, NULL, name, ext);}
	}
}




/*------------------------------------------------------------------------------*
| <<< フォルダー位置を設定 >>>
|	入力	*strDir	= フォルダー名
|			bErr	= true : エラーメッセージ表示
|	戻り値	false :	フォルダー変更失敗
*------------------------------------------------------------------------------*/
bool	sys_folder_set(char	*strDir, bool bErr)
{

	char	strPath[MAX_PATH + 1];

	//--- ファイル名かフォルダー名か調べる ------------------------
	if(strDir != NULL)
	{
		for(int	i =	0;;	i++)
		{
			if(strDir[i] ==	NULL){ break;}

			if(ISKANJI(strDir[i])){ i++;}		// 漢字チェック
			else if(strDir[i] == '\\')			// フォルダーの設定がある場合は末尾を調べて終了
			{
				//if(!YEN_MARK_CHECK(strDir)) strcat(strDir, "\\");
				break;
			}
			else if(strDir[i] == '.')			// ファイル名しかないのでここで終了
			{
				return false;
			}
		}
	}
	//--- フォルダー変更 ------------------------------------------
	if(strDir == NULL)						// フォルダー指定無し
	{
		sprintf(strPath, "%s", sys.strExeFolder);
	}
	else									// フォルダー指定あり
	{
		char	drive[	_MAX_DRIVE + 1];
		char	dir[	_MAX_DIR   + 1];
		char	fname[	_MAX_FNAME + 1];
		char	ext[	_MAX_EXT   + 1];
		_splitpath(strDir, drive, dir, fname, ext);
		if(dir[0] == '\\'){ sprintf(strPath, "%s%s",   drive, dir);}
		else			  { sprintf(strPath, "%s\\%s", drive, dir);}
											// 絶対パスか相対パスか判定
		if(drive[0]	== NULL){ sprintf(strPath, "%s%s\\",	sys.strExeFolder, strDir);}
	}
	//if(_chdir(strPath) == -1)
	//{
	//	if(bErr) return	sys_error(" %s : フォルダー変更失敗", strPath);
	//	else	 return	false;
	//}
	if(PathFileExists(strPath))
	{
		if(SetCurrentDirectory(strPath))
		{
			return true;
		}
		else if(bErr)
		{
			return sys_error(" %s : フォルダー変更失敗", strPath);
		}
	}
	else
	{
		#ifdef	_DEBUG
			return sys_error(" %s : パスが無効です", strPath);
		#endif
	}
	return false;
}



/*----------------------------------------------------------------------*
| <<< 現在のフォルダー位置を保存する >>>
*----------------------------------------------------------------------*/
static	int		iFolderCount;
static	char	strFolder[__FOLDER_MAX][_MAX_PATH +	1];
bool	sys_folder_push(void)
{
	if(iFolderCount	>= __FOLDER_MAX) return	sys_error("SysFolderPush の入れ子数が %d を超えました。", iFolderCount);
	GetCurrentDirectory(MAX_PATH, strFolder[iFolderCount]);
	iFolderCount++;
	return true;
}



/*----------------------------------------------------------------------*
| <<< SysMakePath 実行前のパスへ戻る >>>
*----------------------------------------------------------------------*/
bool	sys_folder_pop(void)
{
	if(iFolderCount	<= 0) return false;//SysError("SysFolderPop にはフォルダー情報が登録されていません。", iFolderCount);
	iFolderCount--;
	SetCurrentDirectory(strFolder[iFolderCount]);
	return true;
}



/*------------------------------------------------------------------------------*
| <<< ディレクトリ取得 >>>
|	入力	hWnd		 = ウィンドゥハンドル
|			lpszFolder	 = 調べるフォルダー名（絶対パス）
|			lpszWildCard = ワイルドカード（*.txtとか）
|			bSw			 = true	: サブフォルダーも調べる
|	出力	strOut		 = 絶対パスを返す
*------------------------------------------------------------------------------*/
static	void	GetDir(char	**strOut, LPCTSTR lpszFolder, LPCTSTR lpszWildCard, bool bSw)
{

	HANDLE			h;
	TCHAR			dir[EXT_MAX][MAX_PATH];
	TCHAR			ext[EXT_MAX][MAX_PATH];
	WIN32_FIND_DATA	fil;
	int				iWildCardCount = 0;

	//--- 連結されている拡張子を分離する --------------------------
	int	k =	0, i;
	for(i =	0;;	i++)
	{
		if(lpszWildCard[i] == NULL || lpszWildCard[i] == ';' ||	lpszWildCard[i]	== '|')
		{
			ext[iWildCardCount][k] = NULL;
			iWildCardCount++;
			if(iWildCardCount >= EXT_MAX)
			{
				sys_error("sys_get_dir 関数内で %d 以上の拡張子で呼び出されました（%d 以内で呼び出してください）。", EXT_MAX, EXT_MAX);
				return;
			}
			if(lpszWildCard[i] == NULL){ break;}
			k =	0;
		}
		else
		{
			ext[iWildCardCount][k] = lpszWildCard[i];
			k++;
		}
	}
	for(i =	0; i < iWildCardCount; i++){ lstrcat(lstrcpy(dir[i], lpszFolder), ext[i]);}

	//--- ファイル検索 --------------------------------------------
	for(i =	0; i < iWildCardCount; i++)
	{
		h =	FindFirstFile(dir[i], &fil);
		if((h =	FindFirstFile(dir[i], &fil)) !=	INVALID_HANDLE_VALUE)
		{
			do
			{
				if((fil.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) !=	FILE_ATTRIBUTE_DIRECTORY)
				{
					sprintf(*strOut, "%s%s", lpszFolder, fil.cFileName);
					*strOut += MAX_PATH;
				}
			}while(FindNextFile(h, &fil) !=	0);
			FindClose(h);
		}
	}

	//--- ディレクトリ検索条件合成 --------------------------------
	lstrcat(lstrcpy(dir[0],	lpszFolder), "*");
	if((h =	FindFirstFile(dir[0], &fil)) ==	INVALID_HANDLE_VALUE){ return;}
	do
	{
		if((fil.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ==	FILE_ATTRIBUTE_DIRECTORY)
		{
			if(strcmp(fil.cFileName, ".") == 0 || strcmp(fil.cFileName,	"..") == 0){ continue;}
											// ディレクトリフルパス合成
			lstrcat(lstrcat(lstrcpy(dir[0],	lpszFolder), fil.cFileName), "\\");
											// サブディレクトリは再帰で処理する
			GetDir(strOut, dir[0], lpszWildCard, bSw);
		}
	}while(FindNextFile(h, &fil) !=	0);

	FindClose(h);
}



/*------------------------------------------------------------------------------*
| <<< ディレクトリを得る >>>
|	入力	lpszFolder	 = 調べるフォルダー名（絶対パス）
|			lpszWildCard = ワイルドカード（*.txtとか）
|			bSw			 = true	: サブフォルダーも調べる
|	出力	strOut		 = 絶対パスを返す
*------------------------------------------------------------------------------*/
void	sys_get_dir(char *strOut, LPCTSTR lpszFolder, LPCTSTR lpszWildCard,	bool bSw)
{
	GetDir(&strOut, lpszFolder, lpszWildCard, bSw);
}



/*------------------------------------------------------------------------------*
| <<< 実行ファイルのディレクトリィ取得 >>>
*------------------------------------------------------------------------------*/
static	void	ExeFolderGet(void)
{
											// カレントディレクトリィの取得
	GetModuleFileName(sys.hInst, sys.strExeFolder, sizeof(sys.strExeFolder));

	char	drive[	_MAX_DRIVE + 1];
	char	dir[	_MAX_DIR   + 1];
	char	fname[	_MAX_FNAME + 1];
	char	ext[	_MAX_EXT   + 1];
	_splitpath(sys.strExeFolder, drive, dir, fname, ext);

	sprintf(sys.strExeFolder, "%s%s", drive, dir);
	return;
}



//--- ファイルデバッグ処理 ------------------------------------------------------------------------

/*------------------------------------------------------------------------------*
| <<< デバッグファイル削除 >>>
*------------------------------------------------------------------------------*/
void	sys_debug_clear(void)
{
	FOLDER_PUSH();
	FOLDER_SET();
	remove(DEBUG_FILE);
	FOLDER_POP();
}



/*------------------------------------------------------------------------------*
| <<< デバッグ情報出力 >>>
|	入力	printf と同じフォーマット
*------------------------------------------------------------------------------*/
void	sys_debug_printf(char *Format,...)
{
	char	*str;
	str = (char*)malloc(10000);

	if(str == NULL){ return;}

	FOLDER_PUSH();
	FOLDER_SET();

	//char	str[512];
	//vsprintf(str, Format, (char	*)(&Format + 1));
	va_list	ap;
	va_start(ap,  Format);
	vsprintf(str, Format, ap);
	va_end(ap);

	FILE	*_Fpt;
	if((_Fpt = fopen(DEBUG_FILE, "at")) != NULL)
	{
		fprintf(_Fpt, "%s\n", str);
		fclose(_Fpt);
	}

	free(str);

	FOLDER_POP();
}


//--- 終了処理 ------------------------------------------------------------------------------------

/*------------------------------------------------------------------------------*
| <<< 強制終了 >>>
*------------------------------------------------------------------------------*/
void	sys_end(char *Format, ...)
{
	if(Format != NULL)
	{
		char	str[512];
		vsprintf(str, Format, (char	*)(&Format + 1));
		FOLDER_SET();
		DEBUG(str);
	}
	SendMessage(sys.hWnd, WM_CLOSE, 0, 0L);
}



/*------------------------------------------------------------------------------*
| <<< アプリに終了を通知する >>>
*------------------------------------------------------------------------------*/
void	sys_exit(void)
{
	SendMessage(sys.hWnd, WM_CLOSE, 0, 0L);
}



//--- ドラッグ＆ドロップ --------------------------------------------------------------------------

/*------------------------------------------------------------------------------*
| <<< ドラッグ＆ドロップ前処理 >>>	※sys.strSelFile にフルパスが入ります。
|	入力	wParam = イベント時の引数
|			strExt = 対象拡張子(.*とすれば全てのファイル)
|			pProc  = 拡張処理(ファイルが複数の時使用のこと)
|				   = NULL(未処理)
|	出力	sys.strSelFile = フルパスが入ります。
|	戻り値	true : ドロップ成功
*------------------------------------------------------------------------------*/
inline	bool	sys_drag_drop_begin(WPARAM wParam, char *strExt, void *pProc)
{

	POINT	pDrop;
	HDROP	hDrop =	(HDROP)wParam;
	bool	bRet  = false;

											// ic にはドロップ数
	int		ic	  =	DragQueryFile(hDrop, 0xFFFFFFFF, sys.strSelFile, MAX_PATH);
	DragQueryPoint(hDrop, &pDrop);

	//STRLWR(sys.strSelFile);				// ファイル名をすべて小文字へ
	//--- 拡張処理をしない場合 --------------------------------------
	if(pProc == NULL)
	{
		int	len	= DragQueryFile(hDrop, 0, sys.strSelFile, MAX_PATH);
		STRLWR(sys.strSelFile);				// ファイル名をすべて小文字へ(↑の命令でせっかくの処理が台無しに)
		sys.strSelFile[len]	= '\0';			// 変数に終端文字を付ける。

		//--- 拡張子が | で複数定義されている場合の処理
		char	*a = strExt;
		while(a!=NULL)
		{
			if(*a == '|') a++;
			char	*b = strstr(a, "|"); 
			char	c[_MAX_EXT + 1];
			memset(c, 0, sizeof(c));
			if(b == NULL) strcpy(c, a);		 // 次に続く拡張子がない場合
			else		  strncpy(c, a, b - a);
			if(strstr(sys.strSelFile, c))	// 拡張子が合致するならば、フォルダーを変更する
			{
				FOLDER_SET(sys.strSelFile);
				bRet = true;
				break;
			}
			a = b;
		}
		//if(strstr(sys.strSelFile, strExt))	// 拡張子が合致するならば、フォルダーを変更する
		//{
		//	FOLDER_SET(sys.strSelFile);
		//	bRet = true;
		//}
	}
	//--- 拡張処理をする場合 ----------------------------------------
	else
	{
		for(int	i =	0; i < ic; i++)			// ドロップされたファイル情報からファイル名を取得
		{
			int	len	= DragQueryFile(hDrop, i, sys.strSelFile, MAX_PATH);
			STRLWR(sys.strSelFile);			// ファイル名をすべて小文字へ(↑の命令でせっかくの処理が台無しに)
			sys.strSelFile[len]	= '\0';		// 変数に終端文字を付ける。
			
			bool	bSw = false;			// 拡張子を調べる
			//if( strstr(strExt,         ".*"))   b = true;
			//if(!strstr(sys.strSelFile, strExt)) b = true;
			//--- 拡張子が | で複数定義されている場合の処理
			char	*a = strExt;
			while(a!=NULL)
			{
				if(*a == '|') a++;
				char	*b = strstr(a, "|"); 
				char	c[_MAX_EXT + 1];
				memset(c, 0, sizeof(c));
				if(b == NULL) strcpy(c, a);	 // 次に続く拡張子がない場合
				else		  strncpy(c, a, b - a);
				if(strstr(sys.strSelFile, c)){ bSw = true; break;}
				if(strstr(c,           ".*")){ bSw = true; break;}
				a = b;
			}
			if(bSw)							// 拡張関数呼び出し
			{
				void(*func)(int, char*);
				func = (void(*)(int, char*))pProc;
				(*func)(i, sys.strSelFile);
			}
		}
	}

	DragFinish(hDrop);						// メモリを開放
	return bRet;
}



/*------------------------------------------------------------------------------*
| <<< ドラッグ＆ドロップ前処理 >>>	※sys.strSelFile にフルパスが入ります。
|	入力	wParam = イベント時の引数
|			strExt = 対象拡張子
|	出力	sys.strSelFile = フルパスが入ります。
|	戻り値	true : ドロップ成功
*------------------------------------------------------------------------------*/
bool	sys_drag_drop_begin(WPARAM wParam, char	*strExt)
{
	return	sys_drag_drop_begin(wParam, strExt, NULL);
}



//--- キー処理 ------------------------------------------------------------------------------------

/*------------------------------------------------------------------------------*
| <<< キー入力 >>>							※DirectInput不要
*------------------------------------------------------------------------------*/
inline	int	sys_key_get(int	iCode)
{
	return(GetAsyncKeyState(iCode) & 0x8000);
}



//--- 最前面の処理	-------------------------------------------------------------------------------

/*------------------------------------------------------------------------------*
| <<< sys.bTopMost = true 時アプリを最前面する >>>
*------------------------------------------------------------------------------*/
void	sys_window_topmost_set(void)
{
	HWND	a;
	if(sys.bTopMost){ a = HWND_TOPMOST;}
	else			{ a = HWND_NOTOPMOST;}
	RECT	b;
	GetWindowRect(sys.hWnd,	&b);
	SetWindowPos( sys.hWnd, a, b.left, b.top, b.right - b.left, b.bottom - b.top, SWP_SHOWWINDOW);
}



//--- ウエイト処理 --------------------------------------------------------------------------------

/*------------------------------------------------------------------------------*
| <<< タイマーリセット処理 >>>
*------------------------------------------------------------------------------*/
void	sys_wait_reset(void)				// 読み込みの後などに行う。
{											// 読み込み中にちゃんと動機を取ってもしょうがないから
	sys.dwTimeMod  = 0;
	sys.dwTimeLast = timeGetTime();
	sys.bFrameSkip = false;
	#ifdef	__SHOW_FPS
		sys.iTimeRateCount = 0;
	#endif
}



/*------------------------------------------------------------------------------*
| <<< タイマー処理 >>>
*------------------------------------------------------------------------------*/
void	sys_wait(void)
{
	#define	_1FRAME_RATE	(1000.f / 60.f)	// １フレームの時間

	sys.iTimeCount++;						// システムカウンター進める
	sys.bFrameSkip = false;

	//--- セカンダリー時のウエイト --------------------------
	#ifdef	__D3
		if(!sys.bPrimary && d3.bWindow)
	#endif
		{
			// ウィンドウがセカンダリーにある場合は vsync が効かないので、
			// タイマーでウエイトを取ります
			DWORD	dw0, dw1;
			for(;;)
			{
				dw0	= timeGetTime();
				dw1	= (dw0 - sys.dwTimeLast) * 3 + sys.dwTimeMod;
				if(dw1 >= 50){ break;}
				Sleep(dw1);
			}
			sys.dwTimeMod  = dw1 - 50;
			//if(dw1 > 1000){ SYS_WAIT_RESET();}// フレームレートが下がった時にリセットしてみる
		}

	//-----------------------------------------------------
	// FPS30 固定の処理
	if(sys.b30Frame)
	{
		if((sys.iTimeCount & 1) == 0)
		{
			for(;;)								// こんな感じでウエイトを取るといいみたい
			{									// 16 は 1000 / 60 の値
				if((timeGetTime() - sys.dwTimeLast) >= (DWORD)(_1FRAME_RATE * 2.f)){ break;}
				Sleep(1);
			}
			sys.bFrameSkip = true;
		}
	}

	// 以下、FPS 計測
	#ifdef	__SHOW_FPS
		DWORD	dw = timeGetTime();				// 今回の時間計測時間
												// バッファへ処理時間を貯めていって
		sys.dwTimeRate[sys.iTimeRateCount] = dw - sys.dwTimeLast;
		sys.iTimeRateCount++;
		if(sys.iTimeRateCount >= TIME_RATE_MAX)	// 適当な時間が来たら計測する
		{
			sys.iTimeRateCount = 0;
			float	a = 0;
			for(int i = 0; i < TIME_RATE_MAX; i++){ a += (float)sys.dwTimeRate[i];}
			float	b = sys.b30Frame ? (float)(TIME_RATE_MAX / 2): (float)TIME_RATE_MAX;
			a = 1000.f / (a / b);
			SYS_CAPTION_PRINTF("%s [FPS:%.2f]", sys.strName, a);
		}
		sys.dwTimeLast = dw;
	#endif
}



/*------------------------------------------------------------------------------*
| <<< スリープ処理 >>>
*------------------------------------------------------------------------------*/
void	sys_sleep(void)
{
	DWORD	dw0, dw1;
	for(;;)
	{
		dw0	= timeGetTime();
		dw1	= (dw0 - sys.dwTimeLast) * 3 + sys.dwTimeMod;
		if(dw1 >= 50){ break;}
		Sleep(dw1);
	}
	sys.dwTimeLast = dw0;
	sys.dwTimeMod  = dw1 - 50;
}



/*------------------------------------------------------------------------------*
| <<< 指定ファイルが更新されているか調べる >>>
|	入力	strFile	= ファイル名
|	出力	pOut	= タイムスタンプ情報
|	戻り値	true : タイムスタンプ更新
*------------------------------------------------------------------------------*/
bool	sys_timestamp_get(FILETIME *pOut, char *strFile)
{

	bool	bRet = false;

	if(strlen(strFile) == 0) return	bRet;

	HANDLE	handle;

	handle = CreateFile(strFile, GENERIC_READ, 0, NULL,	OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS,	NULL);
	if(handle != INVALID_HANDLE_VALUE)
	{
		FILETIME	dum0, dum1,	_ftLastWriteTime; 
		GetFileTime(handle,	&dum0, &dum1, &_ftLastWriteTime);
		CloseHandle(handle);

		if(_ftLastWriteTime.dwLowDateTime  > pOut->dwLowDateTime
		|| _ftLastWriteTime.dwHighDateTime > pOut->dwHighDateTime) *pOut = _ftLastWriteTime, bRet =	true;
	}
	return bRet;
}



/*------------------------------------------------------------------------------*
| <<< ウィンドウスタイルを得る >>>		※この値はsys.hで定義されるため、sys.hのマクロはこの関数を通さないと正しい値は得られない
*------------------------------------------------------------------------------*/
uint	sys_window_style_get(void)
{
	return	WINDOW_STYLE1;
}



/*------------------------------------------------------------------------------*
| <<< ウィンドウ位置がモニターの表示範囲外ならば座標を修正 >>>
|	※マルチモニターの場合は、より表示範囲の大きい方へ移動
*------------------------------------------------------------------------------*/
static	void	FixWindowPos(void)
{

	// 対象モニタの情報を取得
	HMONITOR	hMonitor = MonitorFromRect(&sys.rectWindow, MONITOR_DEFAULTTONEAREST);
	MONITORINFO	mi;
	mi.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(hMonitor, &mi);

	/*
	RECT	rc;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);

	if(mi.rcMonitor.left < rc.left)
	{
		mi.rcMonitor.left = rc.left;
	}
	if(mi.rcMonitor.right > rc.right)
	{
		mi.rcMonitor.right = rc.right;
	}
	if(mi.rcMonitor.top < rc.top)
	{
		mi.rcMonitor.top = rc.top;
	}
	if(mi.rcMonitor.bottom > rc.bottom)
	{
		mi.rcMonitor.bottom = rc.bottom;
	}
	*/

	// 位置補正
	if(sys.rectWindow.right > mi.rcMonitor.right)
	{
		sys.rectWindow.left   -= sys.rectWindow.right  - mi.rcMonitor.right;
		sys.rectWindow.right  = mi.rcMonitor.right;
	}
    if(sys.rectWindow.left < mi.rcMonitor.left)
	{
		sys.rectWindow.right  += mi.rcMonitor.left     - sys.rectWindow.left;
		sys.rectWindow.left    = mi.rcMonitor.left;
    }
	if(sys.rectWindow.bottom > mi.rcMonitor.bottom)
	{
		sys.rectWindow.top    -= sys.rectWindow.bottom - mi.rcMonitor.bottom;
		sys.rectWindow.bottom  = mi.rcMonitor.bottom;
	}
	if(sys.rectWindow.top < mi.rcMonitor.top)
	{
		sys.rectWindow.bottom += mi.rcMonitor.top      - sys.rectWindow.top;
		sys.rectWindow.top     = mi.rcMonitor.top;
	}
}


/*------------------------------------------------------------------------------*
| <<< 諸情報をセーブ >>>
*------------------------------------------------------------------------------*/
static	void	SystemFileSave(void)
{

#ifdef	__SAVE_WINDOW
	DWORD	wr;							// ダミー
	char	str[256];

	// 通常時はウィンドウサイズ取得
	// 最小化、最大化時には前回値を得る
	if(sys.cWindowStyle	== SIZE_RESTORED){ GetWindowRect(sys.hWnd, &sys.rectWindow);}
	else								 { sys.rectWindow = sys.rectWindowRestore;}

	sys_folder_set();					// ディレクトリをＥＸＥ位置に
	remove(SYSTEM_FILE);
	HANDLE	h = CreateFile(SYSTEM_FILE, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, NULL);
	if(h == INVALID_HANDLE_VALUE){ return;}
	/*
	sprintf(str, "%04d,%04d,%04d,%04d,%04d,%04d\r\n%d\r\n%d\r\n",
			sys.rectWindow.top,	   sys.rectWindow.left,		// 右上
			sys.rectWindow.bottom, sys.rectWindow.right,	// 左下
			sys.iWidth,			   sys.iHeight,				// ウィンドウサイズ
			sys.cWindowStyle,								// 最大化なら 2(SIZE_MAXIMIZED)
			sys.bTopMost);									// 1 なら常に最前面
	*/

	// ウィンドウ位置、状態取得
	WINDOWPLACEMENT	w;
	w.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(sys.hWnd, &w);

	// ウィンドウ位置、状態保存(1〜4項目に位置、5項目にZoom状態)
	sprintf(str, "%04d,%04d,%04d,%04d,%04d,%04d\r\n%d\r\n%d\r\n",
		w.rcNormalPosition.top,    w.rcNormalPosition.left,
		w.rcNormalPosition.bottom, w.rcNormalPosition.right,
		w.rcNormalPosition.right  - w.rcNormalPosition.left,	// ウィンドウサイズ
		w.rcNormalPosition.bottom - w.rcNormalPosition.top,
		sys.cWindowStyle,										// 最大化なら 2(SIZE_MAXIMIZED)
		sys.bTopMost);											// 1 なら常に最前面
	WriteFile(h, str, strlen(str), &wr, NULL);
	CloseHandle(h);
#endif
}


/*------------------------------------------------------------------------------*
| <<< SYSTEM_FILE を読み込みウィンドウ位置を復帰 >>>	※無い場合はデスクトップ中央になる
*------------------------------------------------------------------------------*/
static	void	WindowPosLoad(void)
{

#ifdef	__SAVE_WINDOW

	//--- SYSTEM_FILE 読み込み --------------------------------------
	for(;;)
	{
		HANDLE	handle;						// 読み込みハンドル
		DWORD	rs;							// ダミー
		int		size;
		char	str[256];

		FOLDER_SET(NULL, false);
		handle = CreateFile(SYSTEM_FILE, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, NULL);
		if(handle == INVALID_HANDLE_VALUE){			 break;}
		if((size = GetFileSize(handle, NULL)) == 0){ break;}
		memset(str, 0, sizeof(str));
		ReadFile(handle, str, size,	&rs, NULL);
		CloseHandle(handle);
		/*
		sscanf(str,	"%04d,%04d,%04d,%04d,%04d,%04d\r\n%d\r\n%d\r\n",
					&sys.rectWindow.top,	&sys.rectWindow.left,	// 右上
					&sys.rectWindow.bottom,	&sys.rectWindow.right,	// 左下
					&sys.iWidth,			&sys.iHeight,			// ウィンドウサイズ
					&sys.cWindowStyle,								// 最大化なら 2(SIZE_MAXIMIZED)、1 なら最小化
					&sys.bTopMost);									// 1 なら常に最前面
					*/
		// マイナス符号が付いていると sscanf ではきちんと解析してくれないので、以下のようにして文字列をデータ化している
		int	d[8];
		int	j     = 0;
		int	iFlag = 1;
		memset(d, 0, sizeof(d));
		for(int i = 0;; i++)
		{
			if(str[i] == ',' || str[i] == '\r')
			{
				if(str[i + 1] == '\n'){	i++;}
				d[j] *= iFlag;
				j++;
				if(j >= 8){ break;}
				else{      iFlag = 1; continue;}
			}
			else if(str[i] == ' '){				 continue;}
			else if(str[i] == '-'){ iFlag *= -1; continue;}
			else if(str[i] >= '0' && str[i] <= '9')
			{
				d[j] *= 10;
				d[j] += (str[i] - '0');
			}
		}

		// w.length = sizeof(WINDOWPLACEMENT);
		// GetWindowPlacement(hWnd, &w);

		sys.rectWindow.top    = d[0];
		sys.rectWindow.left   = d[1];
		sys.rectWindow.bottom = d[2];
		sys.rectWindow.right  = d[3];
		sys.iWidth            = d[4];
		sys.iHeight           = d[5];
		sys.cWindowStyle      = d[6];
		if(d[7] == 0){ sys.bTopMost = false;}
		else		 { sys.bTopMost = true; }

		// 最小化から始まることはない
		if(sys.cWindowStyle == SIZE_MINIMIZED){ sys.cWindowStyle = SIZE_RESTORED;}

		// 表示範囲補正
		FixWindowPos();
		sys.rectWindowRestore = sys.rectWindow;
		return;
	}
#endif
	//--- デスクトップ中央 ------------------------------------------
	int	iW = GetSystemMetrics(SM_CXSCREEN) / 2 - __SCR_W / 2;
	int	iH = GetSystemMetrics(SM_CYSCREEN) / 2 - __SCR_H / 2;
	SetRect(&sys.rectWindow, iW, iH, iW + __SCR_W, iH + __SCR_H);
	AdjustWindowRect(&sys.rectWindow, WINDOW_STYLE1, true);

	// 表示範囲補正
	FixWindowPos();
	sys.rectWindowRestore = sys.rectWindow;
}



/*------------------------------------------------------------------------------*
| <<< コンフィグファイルの読み書き >>>
|	入力	sw = 0 : load
|			   = 1 : save
*------------------------------------------------------------------------------*/
static	void	config_save_load(int sw)
{

	HANDLE	handle;						// 読み込みハンドル
	DWORD	rs,	wr;						// ダミー
	int		size;						// 読み込みサイズ

	char	*name =	"config.ini";		// コンフィグファイル名
	char	str[2048];
	memset(str,	0, sizeof(str));

	//--- 設定ファイルより諸情報を読み書き ------------------------
	SetCurrentDirectory(sys.strExeFolder);
	if(sw == 1)
	{
		//--- config.ini が必要ない(ConfigSave で文字列が作成されない)場合はファイルを作らない
		ConfigSave(   str);
		size = strlen(str);
		if(size == 0){ return;}

		//--- いったんファイルを削除してから作成
		remove(name);
		handle = CreateFile(name, GENERIC_WRITE, 0,	NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, NULL);
		if(handle == INVALID_HANDLE_VALUE){ return;}
		
		WriteFile(handle, str, size, &wr, NULL);
		for(int	i =	0; i < FILE_BAK; i++)	// 最近使用したファイルを保存
		{
			char	*kai = "\r\n";
			WriteFile(handle, sys.strFileBak[i], strlen(sys.strFileBak[i]),	&wr, NULL);
			WriteFile(handle, kai,				 2,							&wr, NULL);
		}
		char	*n = NULL;
		WriteFile(handle, n, 1,	&wr, NULL);
		CloseHandle(handle);
	}
	else
	{
		bool	bRead =	true;
		handle = CreateFile(name, GENERIC_READ,	0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL |	FILE_FLAG_RANDOM_ACCESS, NULL);
		if(handle == INVALID_HANDLE_VALUE){			 bRead = false;}
		if((size = GetFileSize(handle, NULL)) == 0){ bRead = false;}
		ConfigDefault();
		if(bRead)
		{
			ReadFile(handle, str, sizeof(str), &rs,	NULL);
			int		i =	0;

			if(strncmp(&str[8],	VERSION, 10) ==	0)	// コンフィグファイルのバージョンチェック
			{
				i += 20;
				ConfigLoad(&str[i]);
				{									// 文字を進める
					for(;;)
					{
						if(ISKANJI(str[i])){     i += 2;}
						else if(str[i] == '@'){  break;}
						else if(str[i] == NULL){ break;}
						else{					 i++;}
					}
					i += 3;
				}

													// 最近使用したファイルを読み込み
				for(int	k =	0; k < FILE_BAK; k++)
				{
					if((str[i] == '\r' && str[i	+ 1] ==	'\n') || str[i]	== NULL)
					{
						sys.strFileBak[k][0] = NULL;
						i += 2;
					}
					else
					{
						int	l;
						for(l =	0;;	l++)
						{
							if((str[i +	l] == '\r' && str[i	+ l	+ 1] ==	'\n') || str[i + l]	== NULL){ break;}
							sys.strFileBak[k][l] = str[i + l];
							if(ISKANJI(str[i + l]))	l++, sys.strFileBak[k][l] =	str[i +	l];
						}
						i += l + 2;
						sys.strFileBak[k][l] = NULL;
					}
				}
				MENU_FILE_MAKE();
			}
		}
		if(handle != INVALID_HANDLE_VALUE){ CloseHandle(handle);}
	}
}



/*------------------------------------------------------------------------------*
| <<< マウスの表示管理 >>>
|	入力	sw = true  : 表示オン
|				 false : 表示オフ
*------------------------------------------------------------------------------*/
void	sys_mouse_show(bool	bSW)
{
	if(bSW)
	{
		if(!sys.bMouseShow){ ShowCursor(true);}
		sys.bMouseShow = true;
	}
	else
	{
		if(sys.bMouseShow){ ShowCursor(false);}
		sys.bMouseShow = false;
	}
}



/*------------------------------------------------------------------------------*
| <<< メインプログラムはここから始まる >>>
*------------------------------------------------------------------------------*/
int	 WINAPI	WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{

	// 二重起動禁止
	#ifdef	__MULTIBOOT_BAN
		HANDLE	h = FindWindow(sys.strClass, NULL);
		if(h){ return 0;}
	#endif

	sys.szCmdLine  = szCmdLine;				// コマンドライン
	ExeFolderGet();							// フォルダー位置をＥＸＥのある場所へ

	#ifdef	__NO_WINDOW							// ウィンドウを作らない場合
		if(strlen(szCmdLine) > 0){ game_init();}
		return 0;
	#endif

	MSG			msg;
	WNDCLASSEX	wndclass;

	sys.hInst			   = hInstance;
	wndclass.cbSize		   = sizeof(wndclass);
	wndclass.style		   = WINDOW_STYLE0;
	wndclass.lpfnWndProc   = procWin;
	wndclass.cbClsExtra	   = 0;
	wndclass.cbWndExtra	   = 0;
	wndclass.hInstance	   = hInstance;
	wndclass.hIcon		   = LoadIcon(hInstance, "MAIN");
	wndclass.hCursor	   = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU);
	wndclass.lpszClassName = sys.strClass;
	wndclass.hIconSm	   = LoadIcon(NULL,	"MAIN");

	//--- ウィンドウ色設定 ------------------------------------------
	#ifdef	__WINDOW_COLOR
		wndclass.hbrBackground = (HBRUSH)(__WINDOW_COLOR);
	#else
		wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	#endif
	//---------------------------------------------------------------
	RegisterClassEx(&wndclass);

	sys.bStartUp   = true;					// 初期化フラグオフ
	
	StopScreenSaver();						// スクリーンセーバーを止める

	sys.hAccel	   = LoadAccelerators(sys.hInst, "MYACCEL");
	sys.bMenu	   = true;

	//--- ウィンドウサイズ設定 --------------------------------------
	#ifdef	__WINDOW
		d3.bWindow    = true;				// 起動モードの設定。sys_window_size_set の前で設定のこと
	#else
		d3.bWindow    = false;
	#endif
	WindowPosLoad();						// 内部で SYSTEM_FILE を読み込み前回のウィンドウを得る
	sys.hWnd = CreateWindow(
			sys.strClass,
			sys.strName,
			WINDOW_STYLE1,
			sys.rectWindow.left,
			sys.rectWindow.top,
			sys.rectWindow.right  -	sys.rectWindow.left,
			sys.rectWindow.bottom -	sys.rectWindow.top,
			NULL,
			sys.hMenu,
			hInstance,
			NULL);
	CoInitialize(NULL);

	//--- ウィンドウがデスクトップに収まらないときの処理 ------------
	PrimaryCheck();									// ウィンドウがプライマリーにあるか判定する
	timeBeginPeriod(1);
	sys_window_size_set();							// sys.iWidth, sys.iHeight を設定
	long	bMaximized = (sys.cWindowStyle == SIZE_MAXIMIZED) *	SW_SHOWMAXIMIZED;
	ShowWindow(sys.hWnd, iCmdShow |	bMaximized);	// ウインドウを表示(最大化もチェック)
	if(bMaximized != 0){ sys_window_size_set();}	// 最大化から始まる場合は再度画面サイズを得る
	sys.hMenu		   = GetMenu(sys.hWnd);			// メニューのハンドルを取得
	sys.bMouseShow	   = true;

	#ifndef	__WINCURSOR
		if(d3.bWindow){ sys_mouse_show(false);}		// フルスクリーンはマウスオフ
	#endif

	InitCommonControls();
	snd_init();

	//--- リソース読み込み ------------------------------------------
													// DLLをメモリ上にロード 使用するDLLのパスファイル名
	//sys.hResource	=	::LoadLibrary("共通データ.dll");
	//if(sys.hResource ==	NULL){ SysError("共通データ.dll 読み込み失敗");	goto __END;}

	//--- いろいろ初期化 --------------------------------------------
	config_save_load(0);							// コンフィグより設定の読み出し
	#ifdef	__D3
		if(!d3_init(d3.bWindow))					// DirectGraphics 初期化
		{
			msg.wParam = 0;
			goto __END;
		}
	#endif

	tsk_init();
	pad_init();										// DirectInput の初期化

	#if	__SEPARATO > 0
		MENU_SEPARATOR_ADD();							// メニューにセパレーターを入れる
	#endif

	sys.bFramelateMessage =	true;					// FPS 表示
	if(strlen(szCmdLine) > 0)						// コマンドラインから入力があった場合
	{												// sys.strSelFile にフルパスが入る
		if(szCmdLine[0]	== '"'){ strncpy(sys.strSelFile, &szCmdLine[1], strlen(szCmdLine) - 2);}
		else				   { strcpy( sys.strSelFile, szCmdLine);}
	}
	srand((unsigned)time(NULL));					// 乱数初期化
	sys_window_topmost_set();

	#ifdef	__TABLET
		if(TabletInit() == NULL){ sys_error("タブレットは使えません。");}
	#endif

	game_init();									// ゲーム初期化
	//
	DragAcceptFiles(sys.hWnd, true);				// ファイルドロップを受け付ける
	sys.cResizeCount = DEVICE_MAKE_TIME	+ 1;
	SYS_WAIT_RESET();
	sys.bStartUp     = false;						// 初期化フラグオフ

	//--- ここからメインループ --------------------------------------
	while(true)
	{
		if((PeekMessage(&msg, 0, 0, 0, PM_REMOVE) != 0))
		{  
			if(!TranslateAccelerator(sys.hWnd, sys.hAccel, &msg))   
			{
				if(msg.message == WM_QUIT){ break;}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			// ウィンドウがアクティブかを調べて、ポーズ状態を設定する
			//if(sys.hWnd == GetActiveWindow()){ sys.bActive = true; }
			if(sys.hWnd == GetForegroundWindow()){ sys.bActive = true; }
			else							 { sys.bActive = false;}

			if(     !sys.bActive   ){ sys_pause();}
			else				    { PauseMain();}

#ifndef	__NET
			if(sys.bActive || sys.bTopMost || sys.bAlwayExec)
#endif
			{

#ifdef	__SYSTEM_DEBUG
				if(pad.bKeyPush[KEY_1])		// ストップ
				{
					snd_main();				// サウンドメイン
					if(pad.bKeyClick[KEY_2])// コマ送り
					{
						pad_main();			// キー入力処理
						snd_main();			// サウンドメイン
						AppMain();			// メイン処理
						SYS_WAIT();			// ウエイト処理
					}
					pad_main();				// キー入力処理
					SYS_WAIT_RESET();
				}
				else
				{
					snd_main();				// サウンドメイン
					AppMain();				// メイン処理
					SYS_WAIT();				// ウエイト処理
					pad_main();				// キー入力処理
				}
#else
				snd_main();		// サウンドメイン
				AppMain();		// メイン処理
				SYS_WAIT();		// ウエイト処理
				pad_main();		// キー入力処理
#endif
#ifdef	__SYSTEM_DEBUG
				if(pad.bKeyPush[KEY_TAB])	// 早送り
				{
					sys.bFrameSkip = true;
					AppMain();		// メイン処理
					AppMain();		// メイン処理
					AppMain();		// メイン処理
					sys.bFrameSkip = false;
				}
#endif
			}
#ifndef	__NET
			else
			{
				SYS_WAIT();
				Sleep(1);
				SYS_WAIT_RESET();
			}
#endif
		}
	}
	game_end();								// ゲーム終了処理
	config_save_load(1);					// コンフィグに設定の書き出し
	if(d3.bWindow){ SystemFileSave();}		// ウィンドウモード時にウィンドウサイズ書き出し
	tsk_free();
	snd_end();								// サウンド終了処理
	pad_end();								// DirectInput の終了処理
	#ifdef	__D3
		d3_xfile_free_all();				// Ｘファイル開放
		d3_tex_free_all();					// テクスチャー開放
		d3_end();								// DirectGraphics 終了処理
__END:
	#endif

#ifdef	__CTRL_DEBUG
	ctrl_debug_dialog_close();				// デバッグウィンドウ閉じる
#endif
											// DLLをメモリ上からアンロード
	for(int i = 0; i < SYS_RESOURCE_MAX; i++)
	{
		if(sys.hResource[i] != NULL) ::FreeLibrary(sys.hResource[i]);
	}

	timeEndPeriod(1);						// タイマーの精度をもとにもどす
	sys_mouse_show(true);
	DestroyWindow(sys.hWnd);
	CoUninitialize();
	return msg.wParam;
}


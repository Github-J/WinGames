//
//	メッシュバッファ関係
//

#ifndef	__MESH_BUF_H

#define	__MESH_BUF_H


/*----------------------------------------------------------------------*
| <<< メッシュバッファを初期化する >>>
|	入力	iMax = プライオリティ数(シェーダーを複数種使うときなどに使用)
*----------------------------------------------------------------------*/
extern	void	mesh_buf_init(int iMax);
extern	void	mesh_buf_init(void);

/*----------------------------------------------------------------------*
| <<< メッシュバッファをクリアーする >>>
*----------------------------------------------------------------------*/
extern	void	mesh_buf_clear(void);

/*----------------------------------------------------------------------*
| <<< メッシュを登録する >>>
|	入力	iPri     = プライオリティ番号
|			iXNum	 = モデル番号
|			mtxWorld = 配置座標
|			fAlp	 = アルファ値(0.0f 〜 1.0f)
*----------------------------------------------------------------------*/
extern	void	mesh_buf_set(int iPri, int iXNum, MTX *mtxWorld, float fAlp);
extern	void	mesh_buf_set(          int iXNum, MTX *mtxWorld, float fAlp = 1.0f);

/*----------------------------------------------------------------------*
| <<< バッファのメッシュを描画する >>>
|	入力	iStart, iEnd = クリアーするバッファ番号
*----------------------------------------------------------------------*/
extern	void	mesh_buf_flush(void);

/*----------------------------------------------------------------------*
| <<< バッファのメッシュをソートする >>>
*----------------------------------------------------------------------*/
extern	void	mesh_sort(void);


#endif					// end _mesh_buf.h


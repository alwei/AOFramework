/*************************************************************
*	@file   	SmartComPtr.hpp
*	@brief  	SmartComPtrクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2013/06/13
*************************************************************/

#ifndef _Include_SmartComPtr_hpp_	// インクルードガード
#define _Include_SmartComPtr_hpp_


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  SmartComPtr
	//	@brief  COM専用のスマートポインタ
	//	@author	Tatsunori Aoyama
	//	@date	2013/03/10
	------------------------------------------------------------*/
	template<class T>
	class	SmartComPtr
	{
	public:
		/*!-----------------------------------------------------------
		//	@brief		初期化コンストラクタ
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		SmartComPtr()
		{
			this->pInterface = nullptr;	//NULL代入
			this->refCount = 0;		//参照カウンタ初期化
		}

		/*!-----------------------------------------------------------
		//	@brief		暗黙の型変換禁止によるコンストラクタ
		//	@note		引数で受け取るときにスマートポインタの代入を防ぐ
		//	@param[in]	pInterface COMと予想されるポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		explicit	SmartComPtr(T* pInterface)
		{
			if (pInterface)
				this->refCount = pInterface->AddRef();

			this->pInterface = pInterface;
		}

		/*!-----------------------------------------------------------
		//	@brief		コピーコンストラクタ
		//	@note		参照カウントをインクリメントする
		//	@param[in]	src コピー元のCOM専用スマートポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		SmartComPtr(SmartComPtr& src)
		{
			if (src.pInterface)
				this->refCount = src.pInterface->AddRef();

			this->pInterface = src.pInterface;
		}

		/*!-----------------------------------------------------------
		//	@brief		デストラクタによるCOMポインタの解放
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		~SmartComPtr()
		{
			if (this->pInterface)
				this->refCount = this->pInterface->Release();

			if (this->refCount == 0)
				this->pInterface = nullptr;
		}

		/*!-----------------------------------------------------------
		//	@brief		明示的な代入演算子によるコピー
		//	@note		SmartComPtrのコピーになる
		//	@param[in]	src 右辺の自身と同じ型
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		SmartComPtr	operator=(SmartComPtr& src)
		{
			//コピー先の参照カウント++
			if (src.pInterface)
				this->refCount = src.pInterface->AddRef();

			//自身のポインタは代入によって破棄されるので
			//最後に参照カウントを減らして後は他で制御しているCOMに任せる
			if (this->pInterface)
				this->refCount = this->pInterface->Release();

			if (this->refCount == 0)
				this->pInterface = nullptr;


			this->pInterface = src.pInterface;

			return	(*this);
		}

		/*!-----------------------------------------------------------
		//	@brief		代入演算によるインターフェース登録
		//	@param[in]	pInterface COMポインタによる透過的な代入
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		void	operator=(T* pInterface)
		{
			//自身のCOMポインタを解放し、
			//後は他で制御しているCOMに任せる
			if (this->pInterface)
				this->refCount = this->pInterface->Release();

			this->pInterface = pInterface;
		}

		/*!-----------------------------------------------------------
		//	@brief		比較演算子==
		//	@note		アドレス値の比較
		//	@param[in]	adr アドレス値
		//	@return		引数と一致していたらTRUE、一致していなければFALSE
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		bool	operator==(int adr)
		{
			if (adr == (int)this->pInterface)
				return	true;
			nullptr
				return	false;
		}

		/*!-----------------------------------------------------------
		//	@brief		比較演算子!=
		//	@note		アドレス値の比較
		//	@param[in]	adr アドレス値
		//	@return		引数と一致していたらTRUE、一致していなければFALSE
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		bool	operator!=(int adr)
		{
			if (adr != (int)this->pInterface)
				return	true;

			return	false;
		}

		/*!-----------------------------------------------------------
		//	@brief		比較演算子==
		//	@note		アドレス値の比較
		//	@param[in]	adr アドレス値
		//	@return		引数と一致していたらTRUE、一致していなければFALSE
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		bool	operator==(void* adr)
		{
			if ((int)adr == (int)this->pInterface)
				return	true;
			else
				return	false;
		}

		/*!-----------------------------------------------------------
		//	@brief		比較演算子!=
		//	@note		アドレス値の比較
		//	@param[in]	adr アドレス値
		//	@return		引数と一致していたらTRUE、一致していなければFALSE
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		bool	operator!=(void* adr)
		{
			if ((int)adr != (int)this->pInterface)
				return	true;

			return	false;
		}

		/*!-----------------------------------------------------------
		//	@brief		アロー演算子のオーバーロード
		//	@note		COMインターフェイスの透過性を上げる
		//	@return		インターフェースのポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		T*	operator->(){
			return	this->pInterface;
		}

		/*!-----------------------------------------------------------
		//	@brief		間接参照演算子のオーバーロード
		//	@note		ポインタアクセスによるCOMインターフェースの透過性を上げる
		//	@return		COMのエイリアス
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		T&	operator*(){
			return	(*this->pInterface);
		}

		/*!-----------------------------------------------------------
		//	@brief		配列参照演算子のオーバーロード
		//	@note		インデックス伴奏指定による参照でCOMインターフェースの透過性を上げる
		//	@param[in]	インデックス番号（添え字）
		//	@return		COMのエイリアス
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		T&	operator[](int n){
			return	this->pInterface[n];
		}

		/*!-----------------------------------------------------------
		//	@brief		&演算子をオーバーロード
		//	@note		ポインタ取得の透過性を上げる
		//	@return		COMポインタのアドレス
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		T**	operator&(){
			return	&this->pInterface;
		}

		/*!-----------------------------------------------------------
		//	@brief		現在の参照カウンタ取得
		//	@return		現在のCOMインターフェースの参照カウント
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		unsigned int	GetNowRefCount(){
			return this->refCount;
		}

		/*!-----------------------------------------------------------
		//	@brief		COMインターフェースポインタ取得
		//	@note		明示的に分かりやすいよう関数を用意
		//	@return		COMポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		T*	GetComPtr(){
			return this->pInterface;
		}

		/*!-----------------------------------------------------------
		//	@brief		COMポインタのアドレス取得
		//	@note		使わないかもしれないけど用意してある
		//	@return		COMポインタのアドレス
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		T**	GetComPtrPtr(){
			return &this->pInterface;
		}

		/*!-----------------------------------------------------------
		//	@brief		COMインターフェース生成時に渡す専用関数
		//	@note		明示的にすることにより、使用用途が把握できる
		//	@return		COMポインタのアドレス
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		T**	ToCreator()
		{
			this->refCount = 1;

			//自身のCOMを解放し、
			//後は他で制御しているCOMに任せる
			if (this->pInterface)
				this->refCount = this->pInterface->Release();
			if (this->refCount == 0)
				this->pInterface = nullptr;

			return	&this->pInterface;
		}

		/*!-----------------------------------------------------------
		//	@brief		COMインターフェースの明示的解放
		//	@note		これはあまり使わないでほしい
		//	@author		Tatsunori Aoyama
		//	@date		2013/06/13
		------------------------------------------------------------*/
		int ReleaseComPtr()
		{
			if (this->pInterface)
				this->refCount = this->pInterface->Release();

			if (this->refCount == 0)
				this->pInterface = nullptr;

			return this->refCount;
		}

	private:
		T*				pInterface;		///< インターフェースのテンプレート
		unsigned int	refCount;		///< 参照数
	};
}


#endif // _Include_SmartComPtr_hpp_
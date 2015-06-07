#include"stdafx.hpp"
#include"XFileLoader.hpp"


using namespace ao;


XFileLoader::XFileLoader()
{

}


XFileLoader::~XFileLoader()
{

}



//------------------------------------------------------------------------------
/**	@brief		Xファイルのデータ抽出処理 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2013/10/19	00:11	T.Aoyama	作成
//------------------------------------------------------------------------------
bool	XFileLoader::Load(const xString _filePath)
{
	std::fstream fs;

	fs.open(_filePath.c_str(), std::ios::in);
	if (fs.fail())
		return false;

	char temp;
	std::string	buffer;
	while (fs.get(temp))
		buffer += temp;

	fs.close();

	this->LoadVertex(buffer);
	this->LoadTexcoord(buffer);
	this->LoadIndex(buffer);
	this->LoadMaterial(buffer);
	this->StoreSortIndex();
	this->CreateNormal();

	return true;
}


//------------------------------------------------------------------------------
/**	@brief		stringから頂点データ抽出 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/18	08:01	T.Aoyama	作成
//------------------------------------------------------------------------------
inline	bool	XFileLoader::LoadVertex(const std::string& _buffer)
{
	std::string::size_type index;

	//"Mesh {"の文字列探索	
	index = _buffer.rfind("Mesh {");

	//文字列が見つからない場合抽出失敗
	if (index == std::string::npos)
		return false;

	//数値が見つかるまで探索
	while (!(_buffer.at(index) >= '0' && _buffer.at(index) <= '9'))
		++index;

	//頂点数の文字列抽出
	char temp, numStr[16] = { '\0' };
	for (int i = 0; (temp = _buffer.at(index)) != ';'; ++i, ++index)
		numStr[i] = temp;

	//頂点の数分だけ配列を用意
	int nVertex = atoi(numStr);
	this->serialVertex.resize(nVertex);

	//数値が見つかるまで探索
	while (!(_buffer.at(index) >= '0' && _buffer.at(index) <= '9') && _buffer.at(index) != '-')
		++index;

	//頂点座標を抽出
	int nCount = 0;
	int	nLoopCount = 0;
	std::string	nStrData;
	float store[3];
	int endCode = 0;

	//次の文字列へ
	auto Next = [&](){
		endCode = 0;
	};

	//格納処理
	auto Store = [&](char _n){
		endCode += 0x01;
		if (endCode & 0x02)
			return;
		nStrData += '\0';
		sscanf_s(nStrData.c_str(), "%f", &store[nLoopCount]);
		nStrData.clear();
		nLoopCount = (nLoopCount + 1) % 3;
		if (nLoopCount == 0)
		{
			this->serialVertex[nCount].x = store[0];
			this->serialVertex[nCount].y = store[1];
			this->serialVertex[nCount].z = store[2];
			++nCount;
		}
	};

	//次の文字へ
	auto NextChar = [&](char _n){
		endCode = 0;
		nStrData += _n;
	};

	for (; !(endCode & 0x02);)
	{
		char nChar = _buffer.at(index);

		switch (nChar)
		{
		case ' ':
		case ',':
		case '\n':
			Next();
			break;
		case ';':
			Store(nChar);
			break;
		default:
			NextChar(nChar);
			break;
		}
		++index;
	}

	return true;
}


//------------------------------------------------------------------------------
/**	@brief		テクスチャ座標読み込み */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/19	20:12	T.Aoyama	作成
//------------------------------------------------------------------------------
bool XFileLoader::LoadTexcoord(const std::string& _buffer)
{
	std::string::size_type index;

	//"Mesh {"の文字列探索	
	index = _buffer.rfind("MeshTextureCoords {");

	//文字列が見つからない場合抽出失敗
	if (index == std::string::npos)
		return false;

	//数値が見つかるまで探索
	while (!(_buffer.at(index) >= '0' && _buffer.at(index) <= '9'))
		++index;

	//頂点数の文字列抽出
	char temp, numStr[16] = { '\0' };
	for (int i = 0; (temp = _buffer.at(index)) != ';'; ++i, ++index)
		numStr[i] = temp;

	//頂点の数分だけ配列を用意
	int nTex = atoi(numStr);
	this->serialVertex.resize(nTex);

	//数値が見つかるまで探索
	while (!(_buffer.at(index) >= '0' && _buffer.at(index) <= '9') && _buffer.at(index) != '-')
		++index;

	//頂点座標を抽出
	int nCount = 0;
	int	nLoopCount = 0;
	std::string	nStrData;
	float store[2];
	int endCode = 0;

	//次の文字列へ
	auto Next = [&](){
		endCode = 0;
	};

	//格納処理
	auto Store = [&](char _n){
		endCode += 0x01;
		if (endCode & 0x02)
			return;
		nStrData += '\0';
		sscanf_s(nStrData.c_str(), "%f", &store[nLoopCount]);
		nStrData.clear();
		nLoopCount = (nLoopCount + 1) % 2;
		if (nLoopCount == 0)
		{
			this->serialVertex[nCount].u = store[0];
			this->serialVertex[nCount].v = store[1];
			++nCount;
		}
	};

	//次の文字へ
	auto NextChar = [&](char _n){
		endCode = 0;
		nStrData += _n;
	};

	for (; !(endCode & 0x02);)
	{
		char nChar = _buffer.at(index);

		switch (nChar)
		{
		case ' ':
		case ',':
		case '\n':
			Next();
			break;
		case ';':
			Store(nChar);
			break;
		default:
			NextChar(nChar);
			break;
		}
		++index;
	}

	return true;
}


//------------------------------------------------------------------------------
/**	@brief		stringからインデックスデータ抽出 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/18	08:01	T.Aoyama	作成
//------------------------------------------------------------------------------
inline	bool	XFileLoader::LoadIndex(const std::string& _buffer)
{
	std::string::size_type index;

	//Meshの文字列探索（Indexという文字列がないため）	
	index = _buffer.rfind("Mesh {");

	//文字列が見つからない場合抽出失敗
	if (index == std::string::npos)
		return false;

	//結線数の文字列までシフト
	int isMeshEnd = 0;
	while (!(isMeshEnd & 0x02))
	{
		++index;
		if (_buffer.at(index) == ';')
			isMeshEnd += 0x01;
		else
			isMeshEnd &= 0x00;
	}

	//頂点の終末ダブルセミコロンを飛ばす
	index += 2;

	//インデックス総数までシフト
	while (_buffer.at(index) != ';')
		++index;

	//最初のインデックスの番号が見つかるまでシフト
	while (!(_buffer.at(index) >= '0' && _buffer.at(index) <= '9'))
		++index;

	//結線座標を抽出
	int store[4];
	std::string	nStrData;

	int endCode = 0;
	SortIndex	pushSortIndex;

	//次の文字列へ
	auto Next = [&](){
		endCode = 0;
	};

	//三角形の場合
	auto Triangle = [&](){
		++index;
		sscanf_s(&_buffer[index], "%d,%d,%d", &store[0], &store[1], &store[2]);
		for (int i = 0; i < 3; ++i)
			pushSortIndex.indexArray[i] = store[i];
		pushSortIndex.isQuad = false;
		this->sorter.push_back(pushSortIndex);
	};

	//四角形の場合
	auto Quad = [&](){
		++index;
		sscanf_s(&_buffer[index], "%d,%d,%d,%d", &store[0], &store[1], &store[2], &store[3]);
		for (int i = 0; i < 4; ++i)
			pushSortIndex.indexArray[i] = store[i];
		pushSortIndex.isQuad = true;
		this->sorter.push_back(pushSortIndex);
	};

	//格納処理
	auto Store = [&](){
		endCode += 0x01;
		if (endCode & 0x02)
			return;

		sscanf_s(nStrData.c_str(), "%d", &store[0]);
		switch (store[0])
		{
		case 3:
			Triangle();
			break;
		case 4:
			Quad();
			break;
		}
		nStrData.clear();
		while (_buffer.at(index) != ';')
			++index;
	};

	//次の文字へ
	auto NextChar = [&](char _n){
		endCode = 0;
		nStrData += _n;
	};

	for (; !(endCode & 0x02);)
	{
		char nChar;
		nChar = _buffer.at(index);

		switch (nChar)
		{
		case ' ':
		case ',':
		case '\n':
			Next();
			break;
		case ';':
			Store();
			break;
		default:
			NextChar(nChar);
			break;
		}
		++index;
	}

	return true;
}


//------------------------------------------------------------------------------
/**	@brief		stringからマテリアルデータ抽出 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/18	08:01	T.Aoyama	作成
//------------------------------------------------------------------------------
inline	bool XFileLoader::LoadMaterial(const std::string& _buffer)
{
	std::string::size_type index;

	//"MeshMaterialList {"の文字列探索	
	index = _buffer.rfind("MeshMaterialList {");

	//文字列が見つからない場合抽出失敗
	if (index == std::string::npos)
		return false;

	//数値が見つかるまで探索
	while (!(_buffer.at(index) >= '0' && _buffer.at(index) <= '9'))
		++index;

	//マテリアル数の文字列抽出
	char temp, numStr[16];
	for (int i = 0; (temp = _buffer.at(index)) != ';'; ++i, ++index)
		numStr[i] = temp;

	//マテリアルの数分だけ配列を用意
	int nMaterial = atoi(numStr);
	this->material.resize(nMaterial);

	//文字合わせ
	++index;

	//構成する面数をスキップ
	while (_buffer.at(index) != ';')
		++index;

	//最初の適用する面のマテリアルインデックスまでシフト
	while (!(_buffer.at(index) >= '0' && _buffer.at(index) <= '9'))
		++index;

	int mIndex = 0;

	//ソート用のインデックスデータにマテリアルデータ配置
	for (int i = 0;; ++i)
	{
		sscanf_s(&_buffer[index], "%d", &mIndex);

		//マテリアルのインデックス設定
		this->sorter[i].index = mIndex;

		//マテリアル（サブセット）にインデックス数を加算
		this->material[mIndex].indexCount += (this->sorter[i].isQuad) ? 6 : 3;

		//次へシフト
		++index;

		//次の数値または終端までシフト
		while (!(_buffer.at(index) >= '0' && _buffer.at(index) <= '9') && _buffer.at(index) != ';')
			++index;

		//最後ならbreak
		if (_buffer.at(index) == ';')
			break;
	}

	//"Material"の'M'文字が見つかるまで探索
	while (!(_buffer[index] == 'M'))
		++index;

	//数値が見つかるまで探索
	while (!(_buffer.at(index) >= '0' && _buffer.at(index) <= '9') && _buffer.at(index) != '-')
		++index;

	//マテリアル成分を抽出
	int	nLoopCount = 0;

	for (int i = 0; i < nMaterial; ++i)
	{
		//数字までスキップ処理
		while (!(_buffer.at(index) >= '0' && _buffer.at(index) <= '9'))
			++index;

		//ポインタ格納
		float diffuse[4];
		float specular[4];
		float emissive[3];

		//Diffuse抽出
		sscanf_s(&_buffer[index], "%f;%f;%f;%f", &diffuse[0], &diffuse[1], &diffuse[2], &diffuse[3]);

		//現在の行を飛ばす
		while (_buffer.at(index) != '\n')
			++index;
		//数字までスキップ処理
		while (!(_buffer.at(index) >= '0' && _buffer.at(index) <= '9'))
			++index;
		//スペキュラ強度抽出
		sscanf_s(&_buffer[index], "%f", &specular[3]);

		//現在の行を飛ばす
		while (_buffer.at(index) != '\n')
			++index;
		//数字までスキップ処理
		while (!(_buffer.at(index) >= '0' && _buffer.at(index) <= '9'))
			++index;
		//スペキュラ色抽出
		sscanf_s(&_buffer[index], "%f;%f;%f", &specular[0], &specular[1], &specular[2]);

		//現在の行を飛ばす
		while (_buffer.at(index) != '\n')
			++index;
		//数字までスキップ処理
		while (!(_buffer.at(index) >= '0' && _buffer.at(index) <= '9'))
			++index;
		//自己発光の成分を抽出
		sscanf_s(&_buffer[index], "%f;%f;%f", &emissive[0], &emissive[1], &emissive[2], &emissive[3]);

		//抽出したデータをコピー
		::CopyMemory(this->material[i].diffuse.data(), diffuse, sizeof(float) * 4);
		::CopyMemory(this->material[i].specular.data(), specular, sizeof(float) * 4);
		::CopyMemory(this->material[i].emissive.data(), emissive, sizeof(float) * 3);

		//テクスチャ文字列を検索
		while (!(_buffer.at(index) == '{' || _buffer.at(index) == '}'))
			++index;

		if (_buffer.at(index) == '}')
			continue;

		//ファイルパスまで飛ばす
		while (_buffer.at(index) != '\"')
			++index;

		++index;
		//テクスチャパスを1文字ずつ抽出
		std::string texFilePath;
		while (_buffer.at(index) != '\"'){
			texFilePath += _buffer.at(index);
			++index;
		}

		// テクスチャパスを完全抽出
		this->material[i].texture.assign(texFilePath);
	}

	return true;
}


//------------------------------------------------------------------------------
/**	@brief		頂点データから法線算出 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/18	08:01	T.Aoyama	作成
//------------------------------------------------------------------------------
inline	bool	XFileLoader::CreateNormal()
{
	auto Sub = [=](float3* _pOut, const Vertex& v1, const Vertex& v2){
		_pOut->x = v2.x - v1.x;
		_pOut->y = v2.y - v1.y;
		_pOut->z = v2.z - v1.z;
	};
	auto Cross = [=](float3* _pOut, const float3& _v1, const float3& _v2){
		_pOut->x = (_v1.y*_v2.z - _v1.z*_v2.y);
		_pOut->y = (_v1.z*_v2.x - _v1.x*_v2.z);
		_pOut->z = (_v1.x*_v2.y - _v1.y*_v2.x);
	};
	auto Normalize = [=](float3* _pOut, float3& _v){
		float d = std::sqrtf((_v.x*_v.x) + (_v.y*_v.y) + (_v.z*_v.z));
		_pOut->x = _v.x / d;
		_pOut->y = _v.y / d;
		_pOut->z = _v.z / d;
	};

	for (auto& it : this->serialVertex)
	{
		it.nx = 0;
		it.ny = 0;
		it.nz = 0;
	}

	for (UINT i = 0; i < this->serialIndex.size(); i += 3)
	{
		float3 normal, v1, v2;
		Sub(&v1, this->serialVertex[this->serialIndex[i]], this->serialVertex[this->serialIndex[i + 1]]);
		Sub(&v2, this->serialVertex[this->serialIndex[i]], this->serialVertex[this->serialIndex[i + 2]]);
		Cross(&normal, v1, v2);
		Normalize(&normal, normal);
		for (int j = 0; j < 3; j++){
			this->serialVertex[this->serialIndex[i + j]].nx += normal.x;
			this->serialVertex[this->serialIndex[i + j]].ny += normal.y;
			this->serialVertex[this->serialIndex[i + j]].nz += normal.z;
		}
	}

	for (auto& it : this->serialVertex)
	{
		float3 vN = {it.nx,it.ny,it.nz};
		Normalize(&vN, vN);
		it.nx = vN.x;
		it.ny = vN.y;
		it.nz = vN.z;
	}

	return true;
}


//------------------------------------------------------------------------------
/**	@brief		インデックスをマテリアルインデックスに基づいてソート格納 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2013/10/20	14:56	T.Aoyama	作成
//------------------------------------------------------------------------------
inline	bool XFileLoader::StoreSortIndex()
{
	//インデックス配列ソート（昇順）
	std::sort(this->sorter.begin(), this->sorter.end());

	int iVertex = 0;
	int mLogIndex = 0;
	int iSerial = 0;
	std::vector<INT> vIndex;
	std::vector<Vertex> vList;
	for (auto& it : this->sorter)
	{
		if (mLogIndex != it.index)
		{
			this->sectionIndex.push_back(vIndex);
			vIndex.clear();
		}

		//インデックス格納
		Vertex v[3];
		::CopyMemory( &v[0], &this->serialVertex[it.indexArray[0]], sizeof(Vertex) );
		::CopyMemory( &v[1], &this->serialVertex[it.indexArray[1]], sizeof(Vertex) );
		::CopyMemory( &v[2], &this->serialVertex[it.indexArray[2]], sizeof(Vertex) );
		vList.push_back(v[0]);
		vList.push_back(v[1]);
		vList.push_back(v[2]);
		this->serialIndex.push_back(it.indexArray[0]);
		this->serialIndex.push_back(it.indexArray[1]);
		this->serialIndex.push_back(it.indexArray[2]);
		vIndex.push_back(iSerial+0);
		vIndex.push_back(iSerial+1);
		vIndex.push_back(iSerial+2);

		//四角形なら三角形インデックスを増やす
		if (it.isQuad)
		{
			this->serialIndex.push_back(it.indexArray[0]);
			this->serialIndex.push_back(it.indexArray[2]);
			this->serialIndex.push_back(it.indexArray[3]);
			::CopyMemory(&v[0], &this->serialVertex[it.indexArray[0]], sizeof(Vertex));
			::CopyMemory(&v[1], &this->serialVertex[it.indexArray[2]], sizeof(Vertex));
			::CopyMemory(&v[2], &this->serialVertex[it.indexArray[3]], sizeof(Vertex));
			vList.push_back(v[0]);
			vList.push_back(v[1]);
			vList.push_back(v[2]);
			vIndex.push_back(iSerial + 3);
			vIndex.push_back(iSerial + 4);
			vIndex.push_back(iSerial + 5);
			iSerial += 3;
		}

		mLogIndex = it.index;
		iSerial += 3;
	}

	this->sectionIndex.push_back(vIndex);

	//シリアルインデックスを整理
	iSerial = 0;
	for (auto& it : this->sectionIndex)
	{
		for (auto& idx : it){
			this->serialIndex[iSerial] = idx;
			++iSerial;
		}
	}

	//頂点を三角形分割したものに入れ替え
	this->serialVertex.swap(vList);

	return true;
}
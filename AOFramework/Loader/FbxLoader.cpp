#include"stdafx.hpp"

#if 1
#include"FbxLoader.hpp"
#include"../Math/Math.hpp"
#include"../Utility/Macro.inl"
#include"../Debug/DebugSystem.hpp"


//FBXの行列をXNA算術用の行列に変換
XMFLOAT4X4* FbxMatrixToXMFloat4X4(XMFLOAT4X4* _pOut,const FbxMatrix& _mtx)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			_pOut->m[i][j] = static_cast<float>(_mtx.Get(i, j));
		}
	}

	return _pOut;
}


using namespace ao;


FbxLoader::FbxLoader()
{
	// Initialize the SDK manager. This object handles memory management.
	this->lSdkManager = FbxManager::Create();

	//Create the IO settings object.
	this->ios = FbxIOSettings::Create(this->lSdkManager, IOSROOT);
	this->lSdkManager->SetIOSettings(this->ios);

	//Create an importer using the SDK manager.
	this->lImporter = FbxImporter::Create(this->lSdkManager, "");
}


FbxLoader::~FbxLoader()
{
	this->lSdkManager->Destroy();
}


//------------------------------------------------------------------------------
/**	@brief		読み込み */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/28	16:28	T.Aoyama	作成
//------------------------------------------------------------------------------
bool FbxLoader::Load(const fbxString _filePath,int _loadType)
{
	std::string filePath;
#ifdef UNICODE
	macro::StringWideToMulti(&filePath, _filePath);
#else
	filePath = _filePath;
#endif
	//Use the first argument as the filename for the importer.
	bool isSuccess = this->lImporter->Initialize(filePath.c_str(), -1, this->lSdkManager->GetIOSettings());
	if (!isSuccess){
		debug::PrintfColor(ConsoleColor::H_RED, _T("[FbxLoader::Load]Failed fbx importer initialize \"%s\"\n"), _filePath.c_str());
		return false;
	}

	//Create a new scene so that it can be populated by the importerd file.
	this->lScene = FbxScene::Create(this->lSdkManager, "TestScene");

	//Import the contents of the file into the scene.
	this->lImporter->Import(this->lScene);

	if (lImporter->IsFBX())
	{
		// インポートの状態を設定します。
		// デフォルトでは、インポートの状態は常にtrueに設定されています。
		this->lSdkManager->GetIOSettings()->GetBoolProp(IMP_FBX_MATERIAL, true);
		this->lSdkManager->GetIOSettings()->GetBoolProp(IMP_FBX_TEXTURE, true);
		this->lSdkManager->GetIOSettings()->GetBoolProp(IMP_FBX_LINK, true);
		this->lSdkManager->GetIOSettings()->GetBoolProp(IMP_FBX_SHAPE, true);
		this->lSdkManager->GetIOSettings()->GetBoolProp(IMP_FBX_GOBO, true);
		this->lSdkManager->GetIOSettings()->GetBoolProp(IMP_FBX_ANIMATION, true);
		this->lSdkManager->GetIOSettings()->GetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
	}

	//座標系を変換
	FbxAxisSystem SceneAxisSystem = this->lScene->GetGlobalSettings().GetAxisSystem();
	FbxAxisSystem OurAxisSystem(FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eRightHanded);
	if (SceneAxisSystem != OurAxisSystem)
		OurAxisSystem.ConvertScene(this->lScene);

	//単位系を変換
	FbxSystemUnit SceneSystemUnit = this->lScene->GetGlobalSettings().GetSystemUnit();
	if (SceneSystemUnit.GetScaleFactor() != 1.0)
		FbxSystemUnit::cm.ConvertScene(this->lScene);

	FbxGeometryConverter lGeomConverter(this->lSdkManager);
	// メッシュを三角形に変換
	lGeomConverter.Triangulate(this->lScene, true);
	// マテリアルごとにメッシュを分割
	lGeomConverter.SplitMeshesPerMaterial(this->lScene, true);

	//The file is imported, so get rid if the importer.
	this->lImporter->Destroy();

	//FBX処理↓
	FbxNode* lRootNode = this->lScene->GetRootNode();
	if (lRootNode == nullptr){
		debug::PrintfColor(ConsoleColor::H_RED, _T("[FbxLoader::Load]Failed get root node.\n"));
		return false;
	}

	//初期化
	this->sumVertexCount = 0;
	this->sumIndexCount = 0;
	this->materialCount = 0;
	this->nowMaterialId = 0;
	this->nowMeshId = 0;
	this->indexBuffer.clear();
	this->vertexBuffer.clear();
	this->meshIdBuffer.clear();
	this->uvMeshIdBuffer.clear();
	this->uvVertexBuffer.clear();
	this->vertexCount.clear();
	this->uvVertexCount.clear();
	this->materialBuffer.clear();

	//メッシュ数
	this->numMesh = this->lScene->GetGeometryCount();

	//メッシュ読み込み
	if (_loadType & (Type::Mesh|Type::Motion) )
	{
		//子ノード探索
		for (int i = 0; i < lRootNode->GetChildCount(); ++i)
			this->GetMesh(lRootNode->GetChild(i));

		//メッシュを構築
		this->BuildMesh();
	}

	//メッシュアニメーションの取得
	if( _loadType & Type::Motion )
		this->GetMeshAnimation();

	return true;
}


//------------------------------------------------------------------------------
/**	@brief		フォン情報読み込み */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/29	02:34	T.Aoyama	作成
//------------------------------------------------------------------------------
void FbxLoader::ReadPhongMaterial(FbxMaterial* _pOut, FbxSurfaceMaterial* _pMaterial)
{
	FbxDouble3 diffuse = ((FbxSurfacePhong*)_pMaterial)->Diffuse;
	FbxDouble3 ambient = ((FbxSurfacePhong*)_pMaterial)->Ambient;
	FbxDouble3 specular = ((FbxSurfacePhong*)_pMaterial)->Specular;
	FbxDouble3 emissive = ((FbxSurfacePhong*)_pMaterial)->Emissive;
	FbxDouble transparent = ((FbxSurfacePhong*)_pMaterial)->TransparencyFactor;

	for (int i = 0; i < 3; ++i)
	{
		_pOut->diffuse[i] = (float)diffuse[i];
		_pOut->ambient[i] = (float)ambient[i];
		_pOut->specular[i] = (float)specular[i];
		_pOut->emissive[i] = (float)emissive[i];
	}
	//α値（多分）
	_pOut->diffuse[3] = (float)transparent;

	// テクスチャパスの取得
	// レイヤードテクスチャを取得は今回は考えていません
	// テクスチャ数はディフューズとかスペキュラごとに１つしか対応していません
	FbxProperty prop[3];
	int texCount;

	prop[0] = _pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
	prop[1] = _pMaterial->FindProperty(FbxSurfaceMaterial::sSpecular);
	prop[2] = _pMaterial->FindProperty(FbxSurfaceMaterial::sEmissive);

	std::string* pStr[3];
	pStr[0] = &_pOut->diffuseTexPath;
	pStr[1] = &_pOut->specularTexPath;
	pStr[2] = &_pOut->emissiveTexPath;

	for (int i = 0; i < 3; ++i)
	{
		texCount = prop[i].GetSrcObjectCount<FbxTexture>();
		if (texCount == 0)
			continue;
		for (int texIndex = 0; texIndex < texCount; ++texIndex){
			FbxFileTexture* pTex = prop[i].GetSrcObject<FbxFileTexture>(texIndex);
			pStr[i]->assign(pTex->GetRelativeFileName());
		}
	}
}


//------------------------------------------------------------------------------
/**	@brief		ランバート情報読み込み */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/29	02:35	T.Aoyama	作成
//------------------------------------------------------------------------------
void FbxLoader::ReadLambertMaterial(FbxMaterial* _pOut, FbxSurfaceMaterial* _pMaterial)
{
	FbxDouble3 diffuse = ((FbxSurfaceLambert*)_pMaterial)->Diffuse;
	FbxDouble3 ambient = ((FbxSurfaceLambert*)_pMaterial)->Ambient;
	FbxDouble3 emissive = ((FbxSurfaceLambert*)_pMaterial)->Emissive;
	FbxDouble transparent = ((FbxSurfaceLambert*)_pMaterial)->TransparencyFactor;

	for (int i = 0; i < 3; ++i)
	{
		_pOut->diffuse[i] = (float)diffuse[i];
		_pOut->ambient[i] = (float)ambient[i];
		_pOut->emissive[i] = (float)emissive[i];
		_pOut->specular[i] = 0;
	}
	// アルファ値の取得方法がわからない
	_pOut->diffuse[3] = (float)transparent;

	// テクスチャパスの取得
	// レイヤードテクスチャを取得は今回は考えていません
	// テクスチャ数はディフューズとかスペキュラごとに１つしか対応していません
	FbxProperty prop[2];
	int texCount;

	prop[0] = _pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
	prop[1] = _pMaterial->FindProperty(FbxSurfaceMaterial::sEmissive);

	std::string* pStr[2];
	pStr[0] = &_pOut->diffuseTexPath;
	pStr[1] = &_pOut->emissiveTexPath;

	for (int i = 0; i < 2; ++i)
	{
		texCount = prop[i].GetSrcObjectCount<FbxTexture>();
		if (texCount == 0)
			continue;
		for (int texIndex = 0; texIndex < texCount; ++texIndex){
			FbxFileTexture* pTex = prop[i].GetSrcObject<FbxFileTexture>(texIndex);
			pStr[i]->assign(pTex->GetRelativeFileName());
		}
	}
}


//------------------------------------------------------------------------------
/**	@brief		マテリアル取得 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/29	02:10	T.Aoyama	作成
//------------------------------------------------------------------------------
void FbxLoader::GetMaterial(FbxNode* _node)
{
	if (_node == nullptr){
		debug::PrintfColor(ConsoleColor::H_RED, _T("[FbxLoader::GetMaterial]_node argument is null.\n"));
		return;
	}

	const int numMaterial = _node->GetMaterialCount();
	if (numMaterial == 0){
		debug::PrintfColor(ConsoleColor::H_RED, _T("[FbxLoader::GetMaterial]Material is 0"));
		return;
	}

	auto ReadMaterial = [&](int _index)
	{
		FbxSurfaceMaterial* pMat = _node->GetMaterial(_index);
		for (auto& it : this->materialBuffer){
			if (it.name.compare(pMat->GetName()) == 0)
				return;
		}

		FbxMaterial material;
		material.name = pMat->GetName();
		if (pMat->GetClassId().Is(FbxSurfacePhong::ClassId)){
			this->ReadPhongMaterial(&material, pMat);
		}
		else if (pMat->GetClassId().Is(FbxSurfaceLambert::ClassId)){
			this->ReadLambertMaterial(&material, pMat);
		}

		this->materialBuffer.push_back(material);
		this->vertexCount.push_back(0);
		this->uvVertexCount.push_back(0);
		++this->materialCount;
	};

	for (int i = 0; i < numMaterial; ++i)
		ReadMaterial(i);
}


//------------------------------------------------------------------------------
/**	@brief		メッシュ取得 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/28	17:02	T.Aoyama	作成
//------------------------------------------------------------------------------
void FbxLoader::GetMesh(FbxNode* _node)
{
	if (_node == nullptr){
		debug::PrintfColor(ConsoleColor::H_RED, _T("[FbxLoader::GetMesh]_node argument is null.\n"));
		return;
	}

	//ノードの属性取得
	FbxNodeAttribute* attr = _node->GetNodeAttribute();

	//メッシュのサンプリング
	auto SampleVertex = [&]()
	{
		switch (attr->GetAttributeType())
		{
		case FbxNodeAttribute::eMesh:
			//必要な情報抽出
			this->GetMaterial(_node);
			this->GetGeometry(_node->GetMesh());
			++this->nowMeshId;
			break;
		}
	};

	if (attr)
		SampleVertex();

	//子ノードの再帰探査
	for (int i = 0; i < _node->GetChildCount(); ++i)
		this->GetMesh(_node->GetChild(i));
}


//------------------------------------------------------------------------------
/**	@brief		ジオメトリ情報取得 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/29	03:12	T.Aoyama	作成
//------------------------------------------------------------------------------
void FbxLoader::GetGeometry(FbxMesh* _mesh)
{
	this->nowMaterialId = _mesh->GetElementMaterialCount();
	this->nowMaterialId = this->nowMaterialId - 1;

	this->GetVertex(_mesh);
	this->GetNormal(_mesh);
	this->GetVertexUV(_mesh);
	this->GetWeight(_mesh);

	//頂点オフセット調整
	this->vertexCount[this->nowMaterialId] += _mesh->GetControlPointsCount();
	//UV頂点数加算
	this->uvVertexCount[this->nowMaterialId] += _mesh->GetTextureUVCount();
}


//------------------------------------------------------------------------------
/**	@brief		座標取得 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/28	17:13	T.Aoyama	作成
//------------------------------------------------------------------------------
void FbxLoader::GetVertex(FbxMesh* _mesh)
{
	DWORD id = this->nowMaterialId;

	//UVベースのインデックス処理
	const int indexCount = _mesh->GetPolygonCount();
	for (int i = 0; i < indexCount; ++i){
		this->indexBuffer[id].push_back((DWORD)_mesh->GetTextureUVIndex(i, 0)+this->uvVertexCount[id]);
		this->indexBuffer[id].push_back((DWORD)_mesh->GetTextureUVIndex(i, 1)+this->uvVertexCount[id]);
		this->indexBuffer[id].push_back((DWORD)_mesh->GetTextureUVIndex(i, 2)+this->uvVertexCount[id]);
	}

	this->sumIndexCount += indexCount * 3;

	//頂点データ処理
	const int vCount = _mesh->GetControlPointsCount();
	if (vCount == 0)
	{
		return;
	}

	Vertex vertex;
	for (int i = 0; i < vCount; ++i)
	{
		vertex.x = (float)_mesh->GetControlPointAt(i).mData[0];
		vertex.y = (float)_mesh->GetControlPointAt(i).mData[1];
		vertex.z = (float)_mesh->GetControlPointAt(i).mData[2];
		this->meshIdBuffer[id].push_back(this->nowMeshId);
		this->vertexBuffer[id].push_back(vertex);
	}


	struct uvVertex{
		float x, y, z;
		float index;
	};

	//UVベースの頂点構築
	u32 uvNumVertex = (u32)(this->uvVertexBuffer[id].size() + _mesh->GetTextureUVCount());
	this->uvVertexBuffer[id].resize(uvNumVertex);
	this->uvMeshIdBuffer[id].resize(uvNumVertex);

	this->sumVertexCount += static_cast<DWORD>(vCount);
}


//------------------------------------------------------------------------------
/**	@brief		法線算出 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/28	17:27	T.Aoyama	作成
//------------------------------------------------------------------------------
void FbxLoader::GetNormal(FbxMesh* _mesh)
{
	// 法線レイアウト数を取得
	const int lNormalLayerCount = _mesh->GetElementNormalCount();

	// 法線レイアウト数が１以外の時は対応できていません
	if (lNormalLayerCount != 1){
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[FbxLoader::GetNormal]Normal layout is not 1.(not surpport)\n"));
		return;
	}

	// ここからは法線レイアウトのインデックスが0として処理しています

	// 法線セットの取得
	FbxGeometryElementNormal* pNormal = _mesh->GetElementNormal(0);
	// マッピングモードの取得(メッシュの表面の法線の定義)
	FbxGeometryElement::EMappingMode mapping = pNormal->GetMappingMode();
	// リファレンスモードの取得(データの格納方法を定義)
	FbxGeometryElement::EReferenceMode reference = pNormal->GetReferenceMode();

	//頂点指定の処理２
	auto ControlPointStep02 = [&](int _index)
	{
		// 法線の取得
		float3 vNormal;
		auto normal = pNormal->GetDirectArray().GetAt(_index);
		normal.Normalize();
		vNormal.x = static_cast<float>(normal.mData[0]);
		vNormal.y = static_cast<float>(normal.mData[1]);
		vNormal.z = static_cast<float>(normal.mData[2]);
		DWORD index = _index + this->vertexCount[this->nowMaterialId];
		this->vertexBuffer[this->nowMaterialId][index].nx = vNormal.x;
		this->vertexBuffer[this->nowMaterialId][index].ny = vNormal.y;
		this->vertexBuffer[this->nowMaterialId][index].nz = vNormal.z;
	};

	//頂点指定の処理１
	auto ControlPointStep01 = [&]()
	{
		// 法線数の取得
		const int lNormalCount = pNormal->GetDirectArray().GetCount();

		// eDirectの場合データは順番に格納されているのでそのまま保持
		for (int i = 0; i < lNormalCount; ++i)
			ControlPointStep02(i);
	};

	//頂点指定の処理
	auto StoreControlPoint = [&]()
	{
		// リファレンスモードの判別
		switch (reference)
		{
		case FbxGeometryElement::eDirect:
			ControlPointStep01();
			break;
		};
	};

	//ポリゴン指定の処理２
	auto PolygonVertexStep02 = [&](int _i, int _polyIndex)
	{
		int lNormalIndex = 0;
		int nIndex = _i + _polyIndex * 3;
		if (reference == FbxGeometryElement::eDirect)
			lNormalIndex = nIndex;
		if (reference == FbxGeometryElement::eIndexToDirect)
			lNormalIndex = pNormal->GetIndexArray().GetAt(nIndex);

		// 法線の取得
		float3 vNormal;
		FbxVector4 normal = pNormal->GetDirectArray().GetAt(lNormalIndex);
		normal.Normalize();
		vNormal.x = static_cast<float>(normal.mData[0]);
		vNormal.y = static_cast<float>(normal.mData[1]);
		vNormal.z = static_cast<float>(normal.mData[2]);
		int num = _mesh->GetPolygonVertex(_polyIndex, _i);
		DWORD index = num + this->vertexCount[this->nowMaterialId];
		this->vertexBuffer[this->nowMaterialId][index].nx = vNormal.x;
		this->vertexBuffer[this->nowMaterialId][index].ny = vNormal.y;
		this->vertexBuffer[this->nowMaterialId][index].nz = vNormal.z;
	};

	//ポリゴン指定の処理１
	auto PolygonVertexStep01 = [&](int _polyIndex)
	{
		// ポリゴンサイズの取得
		int lPolygonSize = _mesh->GetPolygonSize(_polyIndex);
		for (int i = 0; i < lPolygonSize; ++i)
			PolygonVertexStep02(i, _polyIndex);
	};

	//ポリゴン指定の処理
	auto StorePolygonVertex = [&]()
	{
		// ポリゴン数の取得
		const int lPolygonCount = _mesh->GetPolygonCount();
		for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; ++lPolygonIndex)
			PolygonVertexStep01(lPolygonIndex);
	};

	// マッピングモードの判別
	switch (mapping)
	{
	case FbxGeometryElement::eByControlPoint:
		StoreControlPoint();
		break;
	case FbxGeometryElement::eByPolygonVertex:
		StorePolygonVertex();
		break;
	}
}


//------------------------------------------------------------------------------
/**	@brief		UV取得 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/28	17:33	T.Aoyama	作成
//------------------------------------------------------------------------------
void FbxLoader::GetVertexUV(FbxMesh* _mesh)
{
	auto eByControlPointDirectStep02 = [&](int _idx, FbxGeometryElementUV* _uv)
	{
		for (int i = 0; i < 3; ++i)
		{
			int ctrlPointIndex = _mesh->GetPolygonVertex(_idx, i);
			int uvIndex = ctrlPointIndex + this->uvVertexCount[this->nowMaterialId];
			DWORD vIndex = uvIndex + this->uvVertexCount[this->nowMaterialId];
			DWORD index = ctrlPointIndex + this->vertexCount[this->nowMaterialId];
			this->uvVertexBuffer[this->nowMaterialId][vIndex].x = this->vertexBuffer[this->nowMaterialId][index].x;
			this->uvVertexBuffer[this->nowMaterialId][vIndex].y = this->vertexBuffer[this->nowMaterialId][index].y;
			this->uvVertexBuffer[this->nowMaterialId][vIndex].z = this->vertexBuffer[this->nowMaterialId][index].z;
			this->uvVertexBuffer[this->nowMaterialId][vIndex].nx = this->vertexBuffer[this->nowMaterialId][index].nx;
			this->uvVertexBuffer[this->nowMaterialId][vIndex].ny = this->vertexBuffer[this->nowMaterialId][index].ny;
			this->uvVertexBuffer[this->nowMaterialId][vIndex].nz = this->vertexBuffer[this->nowMaterialId][index].nz;
			this->uvVertexBuffer[this->nowMaterialId][vIndex].u = (float)(0.0f + _uv->GetDirectArray().GetAt(uvIndex)[0]);
			this->uvVertexBuffer[this->nowMaterialId][vIndex].v = (float)(0.0f + _uv->GetDirectArray().GetAt(uvIndex)[1]);
			this->uvMeshIdBuffer[this->nowMaterialId][vIndex] = this->meshIdBuffer[this->nowMaterialId][index];
		}
	};

	auto eByControlPointIndexToDirectStep02 = [&](int _idx, FbxGeometryElementUV* _uv)
	{
		for (int i = 0; i < 3; ++i)
		{
			int ctrlPointIndex = _mesh->GetPolygonVertex(_idx, i);
			int uvIndex = _uv->GetIndexArray().GetAt(ctrlPointIndex);
			DWORD vIndex = uvIndex + this->uvVertexCount[this->nowMaterialId];
			DWORD index = ctrlPointIndex + this->vertexCount[this->nowMaterialId];
			this->uvVertexBuffer[this->nowMaterialId][vIndex].x = this->vertexBuffer[this->nowMaterialId][index].x;
			this->uvVertexBuffer[this->nowMaterialId][vIndex].y = this->vertexBuffer[this->nowMaterialId][index].y;
			this->uvVertexBuffer[this->nowMaterialId][vIndex].z = this->vertexBuffer[this->nowMaterialId][index].z;
			this->uvVertexBuffer[this->nowMaterialId][vIndex].nx = this->vertexBuffer[this->nowMaterialId][index].nx;
			this->uvVertexBuffer[this->nowMaterialId][vIndex].ny = this->vertexBuffer[this->nowMaterialId][index].ny;
			this->uvVertexBuffer[this->nowMaterialId][vIndex].nz = this->vertexBuffer[this->nowMaterialId][index].nz;
			this->uvVertexBuffer[this->nowMaterialId][vIndex].u = (float)(0.0f + _uv->GetDirectArray().GetAt(uvIndex)[0]);
			this->uvVertexBuffer[this->nowMaterialId][vIndex].v = (float)(0.0f + _uv->GetDirectArray().GetAt(uvIndex)[1]);
			this->uvMeshIdBuffer[this->nowMaterialId][vIndex] = this->meshIdBuffer[this->nowMaterialId][index];
		}
	};

	auto eByControlPointStep01 = [&](FbxGeometryElementUV* _uv, FbxGeometryElement::EReferenceMode _refMode)
	{
		const int polyCount = _mesh->GetPolygonCount();
		for (int index = 0; index < polyCount; ++index)
		{
			switch (_refMode)
			{
			case FbxGeometryElement::eDirect:
				eByControlPointDirectStep02(index, _uv);
				break;
			case FbxGeometryElement::eIndexToDirect:
				eByControlPointIndexToDirectStep02(index, _uv);
				break;
			}
		}
	};

	auto eByPolygonVertexStep02 = [&](int _idx, FbxGeometryElementUV* _uv, FbxGeometryElement::EReferenceMode _refMode)
	{
		for (int i = 0; i < 3; ++i)
		{
			int ctrlPointIndex = _mesh->GetPolygonVertex(_idx, i);
			int uvIndex = 0;
			if (_refMode == FbxGeometryElement::eDirect)
				uvIndex = i + 3 * _idx;
			else
				uvIndex = _uv->GetIndexArray().GetAt(i + 3 * _idx);
			DWORD vIndex = uvIndex + this->uvVertexCount[this->nowMaterialId];
			DWORD index = ctrlPointIndex + this->vertexCount[this->nowMaterialId];
			this->uvVertexBuffer[this->nowMaterialId][vIndex].x = this->vertexBuffer[this->nowMaterialId][index].x;
			this->uvVertexBuffer[this->nowMaterialId][vIndex].y = this->vertexBuffer[this->nowMaterialId][index].y;
			this->uvVertexBuffer[this->nowMaterialId][vIndex].z = this->vertexBuffer[this->nowMaterialId][index].z;
			this->uvVertexBuffer[this->nowMaterialId][vIndex].nx = this->vertexBuffer[this->nowMaterialId][index].nx;
			this->uvVertexBuffer[this->nowMaterialId][vIndex].ny = this->vertexBuffer[this->nowMaterialId][index].ny;
			this->uvVertexBuffer[this->nowMaterialId][vIndex].nz = this->vertexBuffer[this->nowMaterialId][index].nz;
			this->uvVertexBuffer[this->nowMaterialId][vIndex].u = (float)(1.0f - _uv->GetDirectArray().GetAt(uvIndex)[0]);
			this->uvVertexBuffer[this->nowMaterialId][vIndex].v = (float)(1.0f - _uv->GetDirectArray().GetAt(uvIndex)[1]);
			this->uvMeshIdBuffer[this->nowMaterialId][vIndex] = this->meshIdBuffer[this->nowMaterialId][index];
		}
	};

	auto eByPolygonVertexStep01 = [&](FbxGeometryElementUV* _uv, FbxGeometryElement::EReferenceMode _refMode)
	{
		const int polyCount = _mesh->GetPolygonCount();
		for (int index = 0; index < polyCount; ++index)
			eByPolygonVertexStep02(index, _uv, _refMode);
	};

	auto GetUV = [&](int i)
	{
		FbxGeometryElementUV* uv = _mesh->GetElementUV(i);

		//mapping mode
		FbxGeometryElement::EMappingMode mapMode = uv->GetMappingMode();
		//reference mode
		FbxGeometryElement::EReferenceMode refMode = uv->GetReferenceMode();

		//マッピングモードの判別
		switch (mapMode)
		{
		case fbxsdk_2015_1::FbxLayerElement::eNone:
			break;
		case fbxsdk_2015_1::FbxLayerElement::eByControlPoint:
			eByControlPointStep01(uv, refMode);
			break;
		case fbxsdk_2015_1::FbxLayerElement::eByPolygonVertex:
			if (refMode == FbxGeometryElement::eIndexToDirect || refMode == FbxGeometryElement::eDirect)
				eByPolygonVertexStep01(uv, refMode);
			break;
		case fbxsdk_2015_1::FbxLayerElement::eByPolygon:
			break;
		case fbxsdk_2015_1::FbxLayerElement::eByEdge:
			break;
		case fbxsdk_2015_1::FbxLayerElement::eAllSame:
			break;
		default:
			break;
		}

	};

	//UVセット数を取得
	//TODO:UVセット数は1しか対応していない(面倒くさい）
	int uvLayerCount = _mesh->GetElementUVCount();
	for (int i = 0; i < uvLayerCount; ++i)
		GetUV(i);
}


//------------------------------------------------------------------------------
/**	@brief		ウェイト値取得 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/30	09:02	T.Aoyama	作成
//------------------------------------------------------------------------------
void FbxLoader::GetWeight(FbxMesh* _mesh)
{
	FbxSkin* pSkin = reinterpret_cast<FbxSkin*>(_mesh->GetDeformer(0, FbxDeformer::eSkin));

	//メッシュアニメーションの場合
	if (pSkin == nullptr)
	{
		MeshAnimation meshAnim;

		meshAnim.pNode = _mesh->GetNode();
		meshAnim.materialId = this->nowMaterialId;
		this->meshAnimationInfo.push_back(meshAnim);
		return;
	}

	//TODO:スキンメッシュのウェイト読み込み
}


//------------------------------------------------------------------------------
/**	@brief		メッシュアニメーションの取得 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/30	00:08	T.Aoyama	作成
//------------------------------------------------------------------------------
void FbxLoader::GetMeshAnimation()
{
	FbxArray<FbxString*> animStackNameArray;
	this->lScene->FillAnimStackNameArray(animStackNameArray);

	//アニメーションスタック数の取得
	const int animStackCount = animStackNameArray.GetCount();

	//基準時間を取得
	FbxTime period;
	period.SetTime(0, 0, 0, 1, 0, FbxGetGlobalTimeMode());
	this->animation.resize(animStackCount);


	auto StoreFramePattern01 = [&](int _index,FbxTakeInfo* _pInfo)
	{
		FbxTime begin = _pInfo->mLocalTimeSpan.GetStart();
		FbxTime end = _pInfo->mLocalTimeSpan.GetStop();
		this->animation[_index].beginFrame = (int)(begin.Get() / period.Get());
		this->animation[_index].endFrame = (int)(end.Get() / period.Get());
	};

	auto StoreFramePattern02 = [&](int _index)
	{
		FbxTimeSpan timeLineTimeSpan;
		this->lScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(timeLineTimeSpan);
		FbxTime begin = timeLineTimeSpan.GetStart();
		FbxTime end = timeLineTimeSpan.GetStop();
		this->animation[_index].beginFrame = (int)(begin.Get() / period.Get());
		this->animation[_index].endFrame = (int)(end.Get() / period.Get());
	};

	auto ReadMeshAnimation = [&](int _index, Animation* _pAnim, int _frame)
	{
		XMFLOAT4X4 mtx;
		FbxAMatrix fbxMtx = this->meshAnimationInfo[_index].pNode->EvaluateGlobalTransform(period*_frame);
		FbxMatrixToXMFloat4X4((XMFLOAT4X4*)&_pAnim->frame[_frame].mtx[_index], fbxMtx);
	};

	auto ReadAnimation = [&](Animation* _pAnim,int _frame)
	{
		if (this->meshAnimationInfo.empty())
			return;

		_pAnim->frame[_frame].mtx.resize(this->numMesh);
		for (DWORD node = 0; node < this->meshAnimationInfo.size(); ++node)
			ReadMeshAnimation(node, _pAnim, _frame);
	};

	//スタック数だけ処理
	this->enableAnimation = TRUE;
	for (int i = 0; i < animStackCount; ++i)
	{
		//スタック名からアニメーション情報取得
		FbxTakeInfo* pCurrentTakeInfo = this->lScene->GetTakeInfo(*animStackNameArray[i]);
		this->lScene->SetTakeInfo(*pCurrentTakeInfo);
		(pCurrentTakeInfo) ? StoreFramePattern01(i, pCurrentTakeInfo) : StoreFramePattern02(i);

		//アニメーションが正常かどうか（beginFrameに1が入っていたらアニメーションはないと暫定）
		if (this->animation[i].beginFrame == 1){
			this->animation.clear();
			this->enableAnimation = FALSE;
			break;
		}

		//フレーム数格納
		this->animation[i].frameCount = this->animation[i].endFrame - this->animation[i].beginFrame;

		if (this->animation[i].frameCount <= 0){
			this->animation[i].frameCount = 0;
			continue;
		}

		//TODO：アニメーション読み込み実装
		this->animation[i].frame.resize(this->animation[i].frameCount);
		for (int f = 0; f < this->animation[i].frameCount; ++f)
			ReadAnimation(&this->animation[i],f);
	}

	//オブジェクト破棄
	for (int i = 0; i < animStackNameArray.GetCount(); ++i)
		delete animStackNameArray[i];
	animStackNameArray.Clear();
}


//------------------------------------------------------------------------------
/**	@brief		メッシュの構築 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/29	04:07	T.Aoyama	作成
//------------------------------------------------------------------------------
void FbxLoader::BuildMesh()
{
	this->buildMesh.material.resize(this->materialBuffer.size());
	for (DWORD i = 0; i < (DWORD)this->materialCount; i++)
	{
		// 頂点の格納
		for (auto& it : this->uvVertexBuffer[i])
			this->buildMesh.vertex.push_back(it);

		// アニメーションインデックスの格納
		for (auto& it : this->uvMeshIdBuffer[i])
			this->buildMesh.meshIndex.push_back(it);

		// 頂点インデックスの格納
		for (auto& it : this->indexBuffer[i])
		{
			if (i == 0)
				this->buildMesh.index.push_back(it);
			else
				this->buildMesh.index.push_back(it + this->uvVertexCount[i - 1]);
		}

		// マテリアルデータの格納
		if (i == 0)
		{
			this->buildMesh.material[i].indexCount = 0;
		}
		else
		{
			this->buildMesh.material[i].indexOffset = (DWORD)(this->buildMesh.material[i - 1].indexOffset + this->indexBuffer[i - 1].size());
		}

		this->buildMesh.material[i].indexCount = (DWORD)(this->indexBuffer[i].size());
		this->buildMesh.material[i].faceCount = this->buildMesh.material[i].indexCount / 3;

		this->buildMesh.material[i].name = this->materialBuffer[i].name;
		this->buildMesh.material[i].diffuseTexPath = this->materialBuffer[i].diffuseTexPath;
		this->buildMesh.material[i].specularTexPath = this->materialBuffer[i].specularTexPath;
		this->buildMesh.material[i].emissiveTexPath = this->materialBuffer[i].emissiveTexPath;
		::CopyMemory(this->buildMesh.material[i].diffuse.data(), this->materialBuffer[i].diffuse.data(), sizeof(float) * 4);
		::CopyMemory(this->buildMesh.material[i].ambient.data(), this->materialBuffer[i].ambient.data(), sizeof(float) * 3);
		::CopyMemory(this->buildMesh.material[i].specular.data(), this->materialBuffer[i].specular.data(), sizeof(float) * 3);
		::CopyMemory(this->buildMesh.material[i].emissive.data(), this->materialBuffer[i].emissive.data(), sizeof(float) * 3);
	}
}

#endif
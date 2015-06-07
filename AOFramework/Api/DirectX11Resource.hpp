/*************************************************************
*	@file   	DirectX11Resource.hpp
*	@brief  	DirectX11Resource�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/08/01
*************************************************************/

#ifndef _Include_DirectX11Resource_hpp_	// �C���N���[�h�K�[�h
#define _Include_DirectX11Resource_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<map>
#include<mutex>
#include"DirectX11BaseApi.hpp"
#include"../Resource/BaseTexture.hpp"
#include"../Resource/Sprite.hpp"
#include"../Resource/Mesh.hpp"
#include"../Motion/StaMeshMotionController.hpp"
#include"../Define/RenderingDefine.hpp"

namespace ao
{
	class CustomSprite;

	/*!-----------------------------------------------------------
	//	@class  DirectX11Resource
	//	@brief  ���\�[�X�Ǘ��N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/01
	------------------------------------------------------------*/
	class DirectX11Resource : public DirectX11BaseApi
	{
	public:
		DirectX11Resource(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		~DirectX11Resource()override final;

		/*!-----------------------------------------------------------
		//	@brief		������
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		HRESULT	Initialize();

		/*!-----------------------------------------------------------
		//	@brief		�e�N�X�`���̓ǂݍ���
		//	@param[in]	_filePath	�t�@�C���p�X
		//	@param[in]	_filterType	�t�B���^�[�^�C�v[�ȗ���]
		//	@return		�������ꂽ�e�N�X�`���|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		BaseTexture* LoadTextureFromFile(
			const string _filePath,
			const SamplerState _filterType=SamplerState::LINEAR);

		/*!-----------------------------------------------------------
		//	@brief		�X�v���C�g�̓ǂݍ���
		//	@param[in]	_filePath	�t�@�C���p�X
		//	@param[in]	_pivot		�X�v���C�g�̃s�{�b�g[�ȗ���]
		//	@param[in]	_filterType	�t�B���^�[�^�C�v[�ȗ���]
		//	@return		�������ꂽ�X�v���C�g�n���h���|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/16
		------------------------------------------------------------*/
		Sprite*	LoadSpriteFromFile(
			const string _filePath,
			const SpritePivot _pivot=SpritePivot::Center,
			const SamplerState _filterType = SamplerState::LINEAR);

		/*!-----------------------------------------------------------
		//	@brief		UV�w�肠��̃X�v���C�g�ǂݍ���
		//	@param[in]	_filePath	�t�@�C���p�X
		//	@param[in]	_top		����W
		//	@param[in]	_left		�����W
		//	@param[in]	_buttom		�����W
		//	@param[in]	_right		�E���W
		//	@param[in]	_xSize		�S�̂̉��T�C�Y
		//	@param[in]	_ySize		�S�̂̏c�T�C�Y
		//  @param[in]  _useCache	�L���b�V���g�p�t���O[�ȗ���]
		//	@param[in]	_pivot		�X�v���C�g�̃s�{�b�g[�ȗ���]
		//	@param[in]	_filterType	�t�B���^�[�^�C�v[�ȗ���]
		//	@return		�������ꂽ�X�v���C�g�n���h��
		//	@author		Tatsunori Aoyama
		//	@date		2014/11/03
		------------------------------------------------------------*/
		Sprite* LoadSpriteFromFile(
			const string _filePath,
			const int _top, const int _left,
			const int _buttom, const int _right,
			const int _xSize, const int _ySize,
			const BOOL _useCache = FALSE,
			const SpritePivot _pivot = SpritePivot::Center,
			const SamplerState _filterType = SamplerState::LINEAR);

		/*!-----------------------------------------------------------
		//	@brief		�X�v���C�g�̓ǂݍ���
		//	@param[out]	_ppOut		�o�͂���X�v���C�g�z��
		//  @param[in]  _xDiv		��������
		//  @param[in]  _yDiv		�c������
		//	@param[in]	_filePath	�t�@�C���p�X
		//	@param[in]	_pivot		�X�v���C�g�̃s�{�b�g[�ȗ���]
		//	@param[in]	_filterType	�t�B���^�[�^�C�v[�ȗ���]
		//	@return		�������ꂽ�X�v���C�g�z��
		//	@author		Tatsunori Aoyama
		//	@date		2014/11/03
		------------------------------------------------------------*/
		Sprite*	LoadDivSpriteFromFile(
			Sprite** _ppOut,
			int _xDiv,
			int _yDiv,
			const string _filePath,
			const SpritePivot _pivot = SpritePivot::Center,
			const SamplerState _filterType = SamplerState::LINEAR);

		/*!-----------------------------------------------------------
		//	@brief		X�t�@�C���̓ǂݍ���
		//	@param[in]	_filePath	�t�@�C���p�X
		//  @param[in]  _edgeType	�G�b�W�^�C�v[�ȗ���]
		//	@return		�������ꂽ���b�V���|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/19
		------------------------------------------------------------*/
		StMesh* LoadMeshFromX(const string _filePath);

		/*!-----------------------------------------------------------
		//	@brief		PMD�̓ǂݍ���
		//	@param[in]	_filePath		�t�@�C���p�X
		//	@param[in]	_meshType		���b�V���^�C�v
		//  @param[in]  _enableFixPath	�p�X�C���̗L��[�ȗ���]
		//	@return		�������ꂽ���b�V���|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/03
		------------------------------------------------------------*/
		BaseMesh* LoadMeshFromPmd(
			const string _filePath,
			const ResourceType _meshType,
			const BOOL _enableFixPath=TRUE);

		/*!-----------------------------------------------------------
		//	@brief		Obj�t�@�C���̓ǂݍ���
		//	@note		�o�O���Ă���̂œ�����
		//	@param[in]	_filePath	�t�@�C���p�X
		//	@param[in]	_mtlPath	MTL�t�@�C���p�X[�ȗ���]
		//	@return		�������ꂽ���b�V���|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/26
		------------------------------------------------------------*/
		StMesh* LoadMeshFromObj(const string _filePath, const string _mtlPath = _T(""));

		/*!-----------------------------------------------------------
		//	@brief		FBX�̓ǂݍ���
		//	@param[in]	_filePath		�t�@�C���p�X
		//  @param[in]  _meshType		���b�V���^�C�v
		//  @param[in]  _enablefixPath	�p�X�̏C���t���O[�ȗ���]
		//  @param[out] _pMotionCtrl	�o�͂��郂�[�V�����R���g���[���[[�ȗ���]
		//  @return     �������ꂽ���b�V���|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/10
		------------------------------------------------------------*/
		BaseMesh* LoadMeshFromFbx(
			const string _filePath,
			const ResourceType _meshType,
			const BOOL enableFixPath=TRUE,
			IMeshMotionController* _pMotionCtrl = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		FBX���[�V������ǂݍ���
		//	@return		_pOut		�o�͂��郂�[�V�����R���g���[���[
		//	@param[in]	_filePath	�t�@�C���p�X
		//	@param[in]	_meshType	���b�V���^�C�v
		//	@param[in]	_isAdd		�ǉ��t���O[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2014/11/03
		------------------------------------------------------------*/
		HRESULT	LoadMotionFromFbx(
			IMeshMotionController* _pOut,
			const string _filePath,
			const ResourceType _meshType,
			const BOOL _isAdd=TRUE);

		/*!-----------------------------------------------------------
		//	@brief		�{�b�N�X���b�V���ǂݍ���
		//	@return		�������ꂽBox���b�V��
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/14
		------------------------------------------------------------*/
		StMesh* LoadBoxMesh();

		/*!-----------------------------------------------------------
		//	@brief		�X�t�B�A���b�V���ǂݍ���
		//	@param[in]	_division ������[�ȗ���]
		//	@return		�������ꂽ�X�t�B�A���b�V��
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/14
		------------------------------------------------------------*/
		StMesh* LoadSphereMesh(int _division = 16);

	private:
		typedef std::vector<Sprite*> SpriteArray;

	private:
		SmartComPtr<ID3D11SamplerState> pDefaultSamplerState[(int)SamplerState::MAX];
		Sprite leftTopSprite;
		Sprite centerSprite;
		std::map<string,BaseTexture*> pTextureMap;
		std::map<string, Sprite*> pSpriteMap;
		std::map<string, SpriteArray*> pSpriteArrayMap;
		std::map<string, BaseMesh*> pMeshMap;
		std::map<string, StaMotionData*> pStaMotionMap;
		std::mutex textureMutex;
		std::mutex spriteMutex;
		std::mutex meshMutex;

	private:
		/*!-----------------------------------------------------------
		//	@brief		�T���v���[�X�e�[�g�쐬
		//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		inline	HRESULT	CreateDefaultSamplerState();

		/*!-----------------------------------------------------------
		//	@brief		���_�o�b�t�@�쐬
		//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/16
		------------------------------------------------------------*/
		inline	HRESULT	CreateSpriteVertexBuffer();
	};
}

#endif // _Include_DirectX11Resource_hpp_
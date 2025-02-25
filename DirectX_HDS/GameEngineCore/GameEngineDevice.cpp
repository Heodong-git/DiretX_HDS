#include "PrecompileHeader.h"
#include "GameEngineDevice.h"
#include <GameEngineBase/GameEngineDebug.h>
#include "GameEngineRenderTarget.h"

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "DXGI")

ID3D11Device* GameEngineDevice::Device = nullptr;
ID3D11DeviceContext* GameEngineDevice::Context = nullptr;
IDXGISwapChain* GameEngineDevice::SwapChain = nullptr;
std::shared_ptr<GameEngineRenderTarget> GameEngineDevice::BackBufferTarget = nullptr;

//ID3D11Texture2D* GameEngineDevice::BackBufferTexture = nullptr;
//ID3D11RenderTargetView* GameEngineDevice::RenderTarget = nullptr;


GameEngineDevice::GameEngineDevice()
{
}

GameEngineDevice::~GameEngineDevice()
{
}

IDXGIAdapter* GameEngineDevice::GetHighPerformanceAdapter()
{
	// GDI+ DXGI <=

	IDXGIFactory* Factory = nullptr;
	IDXGIAdapter* Adapter = nullptr;

	// c++에서 지원하는 클래스를 구분하기 위한 GUI를 얻어오는 
	// MIDL_INTERFACE("7b7166ec-21c7-44ae-b21a-c9ae321ae369")
	HRESULT HR = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&Factory);

	if (nullptr == Factory)
	{
		MsgAssert("그래픽카드에서 팩토리 인터페이스를 생성하지 못했습니다.");
		return nullptr;
	}

	size_t prevAdapterVideoMemory = 0;

	for (UINT Adapterindex = 0; ; Adapterindex++)
	{
		IDXGIAdapter* CurAdapter = nullptr;

		Factory->EnumAdapters(Adapterindex, &CurAdapter);
		if (nullptr == CurAdapter)
		{
			break;
		}

		DXGI_ADAPTER_DESC Desc;
		CurAdapter->GetDesc(&Desc);

		if (prevAdapterVideoMemory <= Desc.DedicatedVideoMemory)
		{
			prevAdapterVideoMemory = Desc.DedicatedVideoMemory;

			if (nullptr != Adapter)
			{
				Adapter->Release();
			}

			Adapter = CurAdapter;
			continue;
		}

		CurAdapter->Release();
	}

	Factory->Release();

	return Adapter;

}

void GameEngineDevice::CreateSwapChain()
{
	float4 ScreenSize = GameEngineWindow::GetScreenSize();

	DXGI_SWAP_CHAIN_DESC SwapChainDesc = { 0, };

	// 기본정보
	SwapChainDesc.BufferCount = 2;
	SwapChainDesc.BufferDesc.Width = ScreenSize.uix();
	SwapChainDesc.BufferDesc.Height = ScreenSize.uiy();
	SwapChainDesc.OutputWindow = GameEngineWindow::GetHWnd();

	// 화면 갱신률
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;

	// 그래픽이미지 포맷
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	// 뭐였지?
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 이 스왑체인은 단순히 
	// DXGI_USAGE_RENDER_TARGET_OUTPUT 윈도우에 띄워주는 용도로 만들것이다.
	// 쉐이더에서도 이걸 사용할수 있게 하겠다.
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;

	// 안티앨리어싱
	SwapChainDesc.SampleDesc.Quality = 0;
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

    // 화면조정을 위해 들어간 기능, 굳이 더 알려고 할 필요 X
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// true - 창 , false - 전체
	SwapChainDesc.Windowed = true;

	IDXGIDevice* SwapDevice = nullptr;
	IDXGIAdapter* SwapAdapter = nullptr;
	IDXGIFactory* SwapFactory = nullptr;

	Device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&SwapDevice));
	if (nullptr == SwapDevice)
	{
		MsgAssert("DXGI 디바이스를 DirectX디바이스에서 얻어오지 못했습니다.");
		return;
	}

	// Find("") 에서 string넣어서 찾아야하는 것처럼
	// 디바이스에서는 내부에 가지고 있는 포인터나 맴버변수를 얻어오려면
	// __uuidof(IDXGIAdapter) 같은 GUID를 넣어줘야 한다.
	// 프로그램을 통틀어서 단 1개만 존재할수 있는 키를 만드는 기법
	// MIDL_INTERFACE("2411e7e1-12ac-4ccf-bd14-9798e8534dc0")
	SwapDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&SwapAdapter));
	if (nullptr == SwapAdapter)
	{
		MsgAssert("DXGI 디바이스를 DirectX디바이스에서 얻어오지 못했습니다.");
		return;
	}

	SwapAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&SwapFactory));

	if (S_OK != SwapFactory->CreateSwapChain(Device, &SwapChainDesc, &SwapChain))
	{
		MsgAssert("스왑체인 생성에 실패했습니다.");
		return;
	}

	SwapDevice->Release();
	SwapAdapter->Release();
	SwapFactory->Release();

	ID3D11Texture2D* SwapBackBufferTexture = nullptr;

	HRESULT Result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&SwapBackBufferTexture));
	if (S_OK != Result)
	{
		MsgAssert("스왑체인 생성에 실패했습니다.");
		return;
	}

	std::shared_ptr<GameEngineTexture> BackBufferTexture = std::make_shared<GameEngineTexture>();
	BackBufferTexture->ResCreate(SwapBackBufferTexture);

	BackBufferTarget = GameEngineRenderTarget::Create("MainBackBufferTarget", BackBufferTexture, { 0.0f, 0.0f, 1.0f, 1.0f });
	BackBufferTarget->CreateDepthTexture();
}

void GameEngineDevice::RenderStart()
{
	BackBufferTarget->Clear();
	BackBufferTarget->Setting();
}

void GameEngineDevice::RenderEnd()
{
	HRESULT Result = SwapChain->Present(0, 0);
	if (Result == DXGI_ERROR_DEVICE_REMOVED || Result == DXGI_ERROR_DEVICE_RESET)
	{
		// 디바이스 다시만들기
		MsgAssert("랜더타겟 생성에 실패했습니다.");
		return;
	}

}

void GameEngineDevice::Initialize()
{
	if (nullptr == GameEngineWindow::GetHWnd())
	{
		MsgAssert("윈도우가 만들어지지 않았는데 디바이스를 초가화 할수는 없습니다.");
		return;
	}

	int iFlag = 0;

#ifdef _DEBUG
	// 다이렉트x도 디버그 기능을 지원
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL Level = D3D_FEATURE_LEVEL_11_0;

	// 그래픽카드와 직접연결되는 인터페이스
	IDXGIAdapter* Adapter = GetHighPerformanceAdapter();

	if (nullptr == Adapter)
	{
		MsgAssert("그래픽카드 장치 어뎁터 인터페이스를 얻어오는데 실패했습니다.");
		return;
	}

	// CPU로 그려
	// D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_SOFTWARE

	// 그래픽카드로 찾아서 그려줘.
	// D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE

	// sdk 소프트웨어 디벨롭먼트 키트
	// 즉개발자에게 제공되는 lib header 라이브러리들의 총집합을 xxxx SDK

	// D3D11_SDK_VERSION 그냥 이 윈도우에서 지원하는 sdk 버전이 define

	HRESULT Result = D3D11CreateDevice(
		Adapter,
		D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_UNKNOWN,
		nullptr,
		iFlag,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&Device,
		&Level,
		&Context
	);

	if (S_OK != Result)
	{
		MsgAssert("디바이스 생성에 실패했습니다.");
		return;
	}

	if (nullptr != Adapter)
	{
		Adapter->Release();
		Adapter = nullptr;
	}

	// 최종적으로 결정된 다이렉트 레벨이 여기로 넘어올 것이다.
	// Level

	if (Level != D3D_FEATURE_LEVEL_11_0)
	{
		MsgAssert("다이렉트 11을 지원하지 않는 그래픽카드 입니다");
		return;
	}

	// 윈도우와 연결하는 작업
	// 즉 백버퍼 만드는 작업을 하게 된다.
	// 다이렉트 x에서 멀티쓰레드 관련 
	// 멀티스레드를 사용하겠다는 설정
	Result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	CreateSwapChain();
}

void GameEngineDevice::Release()
{
	BackBufferTarget = nullptr;

	if (nullptr != SwapChain)
	{
		SwapChain->Release();
		SwapChain = nullptr;
	}

	if (nullptr != Context)
	{
		Context->Release();
		// 임시적용 
		Context->Flush();
		Context = nullptr;
	}

	if (nullptr != Device)
	{
		Device->Release();
		Device = nullptr;
	}
}
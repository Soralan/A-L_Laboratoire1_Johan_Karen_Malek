#pragma once
#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "ole32.lib")
#include <windows.h>
#include <iostream>
#include <dshow.h>

void main(void)
{
	IGraphBuilder *pGraph = NULL;
	IMediaControl *pControl = NULL;
	IMediaEvent   *pEvent = NULL;
	IMediaSeeking *pSeeking = NULL;

	// Initialize the COM library.
	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		printf("ERROR - Could not initialize COM library");
		return;
	}

	// Create the filter graph manager and query for interfaces.
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
		IID_IGraphBuilder, (void **)&pGraph);
	if (FAILED(hr))
	{
		printf("ERROR - Could not create the Filter Graph Manager.");
		return;
	}

	hr = pGraph->QueryInterface(IID_IMediaControl, (void **)&pControl);
	hr = pGraph->QueryInterface(IID_IMediaEvent, (void **)&pEvent);
	hr = pGraph->QueryInterface(IID_IMediaSeeking, (void **)&pSeeking);

	// Build the graph. IMPORTANT: Change this string to a file on your system.
	hr = pGraph->RenderFile(L"rue.avi", NULL);

	char choix;
	bool actif = 0;

	if (SUCCEEDED(hr))
	{
		do {

			std::cout << "\n P.Play ou pause \n A. Avance rapide \n R. Retour \n Q. Quitter \n Entree: ";
			std::cin >> choix;

			LONGLONG param1 = 5;
			LONGLONG param2 = 6;

			switch (choix) {
			case 'P':
				if (actif) {
					hr = pControl->Pause();
					actif = 0;
					break;
				}
				// Run the graph.
				hr = pControl->Run();
				actif = 1;
				break;
			case 'A':
				hr = pSeeking->SetRate(2.0);
				break;
			case 'R':
				//hr = pSeeking->SetPositions(&param1, 0, &param2, 0);
				//HRESULT hr = pControl->Run();
				if (SUCCEEDED(hr))
				{

					REFERENCE_TIME rtNow = 0,
						//met la video a la position de depart
						hr = pSeeking->SetPositions(
							&rtNow, AM_SEEKING_AbsolutePositioning,
							NULL, AM_SEEKING_NoPositioning
						);
				}

				break;
			case 'Q':
				hr = pControl->Stop();
				break;
			default: std::cout << "bad insert" << std::endl;
			}

		} while (choix != 'Q');
	}

	pControl->Release();
	pEvent->Release();
	pGraph->Release();
	pSeeking->Release();
	CoUninitialize();
}
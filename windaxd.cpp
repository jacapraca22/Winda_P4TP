// windaxd.cpp : Definiuje punkt wejścia dla aplikacji.
//

#include "framework.h"
#include "windaxd.h"

#define MAX_LOADSTRING 100


#define predkosc 5

class osoba
{
public:
    osoba(int x, int y, int cel)
    {
        this->cel = cel;
        this->x = x;
        this->y = y;
        this->waga = 70;
        this->kierunek = 's';//s-stop,g-gora,d-dol,l-lewo,p-prawo
        
    }
    int x;
    int y;
    int cel;
    int waga;
    char kierunek;
    int xCel;
};

std::vector<osoba> kolejka[5];

class WINDA {
    public:
    WINDA() {
        y = 0;
        kierunek = 1;
        stop = 0;
        waga = 0;
        cel = 0;
        pietro = 0;
    }

    int wybierz_pietro(int cel) {
        stop = 0;
        this->cel = cel;
        return cel;
    }

    void ruch(std::vector<osoba> *kolejka) {
              
       
        if (y < cel * 125 - predkosc) {
            y += predkosc;
            for (auto& osoba : osobywwindzie) {
                osoba.y -= predkosc;
            }
        }
        else if (y > cel * 125 + predkosc) {
            y -= predkosc;
            for (auto& osoba : osobywwindzie) {
                osoba.y += predkosc;
            }
        }
        else {
            pietro = cel;
            stop = 1;
            for (auto& osoba : kolejka[pietro])
            {
                if (osoba.kierunek == 's')
                {
                    if (pietro % 2 == 0)
                    {
                        osoba.kierunek = 'p';
                        osoba.xCel = osoba.x + 250;
                    }
                    else
                    {
                        osoba.kierunek = 'l';
                        osoba.xCel = osoba.x - 250;
                    }
                }
                    

            }
            for (auto& osoba : osobywwindzie) {
                if (osoba.cel == pietro) {
                    if (pietro % 2 == 0)
                    {
                        osoba.kierunek = 'l';
                        osoba.xCel = osoba.x - 250;
                    }
                    else
                    {
                        osoba.kierunek = 'p';
                        osoba.xCel = osoba.x + 250;
                    }

                    kolejka[pietro].push_back(osoba);
                }               
            }
            osobywwindzie.erase(
                std::remove_if(
                    osobywwindzie.begin(),
                    osobywwindzie.end(),
                    [&](osoba const& osoba) { return (osoba.cel==pietro); }
                ),
                osobywwindzie.end()
            );
        }
        for (auto& osoba : kolejka[pietro])
        {
            switch (osoba.kierunek)
            {
            case 'l':
                osoba.x -= predkosc;
                if (osoba.x <= osoba.xCel && osoba.cel != pietro)
                {
                    osobywwindzie.push_back(osoba);
                    cel = osoba.cel;
                }break;
            case 'p':
                osoba.x += predkosc;
                if (osoba.x >= osoba.xCel && osoba.cel != pietro)
                {
                    osobywwindzie.push_back(osoba);
                    cel = osoba.cel;
                }break;
            }

        }
        //algorytm usuwajacy ludka z pietra
        kolejka[pietro].erase(
            std::remove_if(
                kolejka[pietro].begin(),
                kolejka[pietro].end(),
                [](osoba const& osoba) { return (osoba.x <= osoba.xCel && osoba.kierunek == 'l') || (osoba.x >= osoba.xCel && osoba.kierunek == 'p'); }
            ),
            kolejka[pietro].end()
        );
    }
    std::vector<osoba> osobywwindzie;
    int y;
    bool kierunek;
    bool stop;
    int waga;
    int max_waga;
    int pietro;
    int cel;
};




// Zmienne globalne:
HINSTANCE hInst;                                // bieżące wystąpienie
WCHAR szTitle[MAX_LOADSTRING];                  // Tekst paska tytułu
WCHAR szWindowClass[MAX_LOADSTRING];            // nazwa klasy okna głównego
WINDA winda;

// Przekaż dalej deklaracje funkcji dołączone w tym module kodu:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void Rysuj(HDC HDC);
void przycisk(int buttonid)
{
    int cel;
    int pietro;
    cel = buttonid % 10;
    pietro = buttonid / 10;
    int x = (pietro % 2) * 600;
    int y = 625 - (pietro / 2) * 2 * 125 - (pietro % 2) * 125 - 512 / 6;
    osoba ludek(x, y, cel);
    kolejka[pietro].push_back(ludek);
    winda.wybierz_pietro(pietro);

}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: W tym miejscu umieść kod.

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // Inicjuj ciągi globalne

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDAXD, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Wykonaj inicjowanie aplikacji:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDAXD));

    MSG msg;

    // Główna pętla komunikatów:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNKCJA: MyRegisterClass()
//
//  PRZEZNACZENIE: Rejestruje klasę okna.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDAXD));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDAXD);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNKCJA: InitInstance(HINSTANCE, int)
//
//   PRZEZNACZENIE: Zapisuje dojście wystąpienia i tworzy okno główne
//
//   KOMENTARZE:
//
//        W tej funkcji dojście wystąpienia jest zapisywane w zmiennej globalnej i
//        jest tworzone i wyświetlane okno główne programu.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Przechowuj dojście wystąpienia w naszej zmiennej globalnej

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   SetTimer(hWnd, 1, 33, NULL);    //33 uElapse = około 30 fps

   HWND hwndButton = CreateWindow(
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"EXIT",      // Button text 
       WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
       800,         // x position 
       0,           // y position 
       50,        // Button width
       20,        // Button height
       hWnd,     // Parent window
       (HMENU)555, 
       (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   for (int i = 0; i <= 4; i++) {
       for (int przycisk_i = 4; przycisk_i >= 0; przycisk_i--) {

           int pietro = abs(i - 4);
           if (pietro == przycisk_i) continue;
           wchar_t buffer[256];
           wsprintfW(buffer, L"%d", przycisk_i);
           HWND hwndButton = CreateWindow(
               L"BUTTON",  // Predefined class; Unicode assumed 
               buffer,      // Button text 
               WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
               1 + (i % 2) * 655,         // x position 
               125 + (i / 2) * 2 * 125 - (przycisk_i + 2) * 20 + (i % 2) * 125,  // y position 
               20,        // Button width
               20,        // Button height
               hWnd,     // Parent window
               (HMENU)(pietro * 10 + przycisk_i), //miejsce, cel
               (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
               NULL);      // Pointer not needed.
       }
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


//
//  FUNKCJA: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PRZEZNACZENIE: Przetwarza komunikaty dla okna głównego.
//
//  WM_COMMAND  - przetwarzaj menu aplikacji
//  WM_PAINT    - Maluj okno główne
//  WM_DESTROY  - opublikuj komunikat o wyjściu i wróć
//
//


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            for (int i = 0; i <= 4; i++) {
                for (int przycisk_i = 4; przycisk_i >= 0; przycisk_i--) {
                    int pietro = abs(i - 4);
                    if (pietro == przycisk_i) continue;
                    if (wmId == pietro * 10 + przycisk_i) 
                    przycisk(wmId);

                }
            }
            // Analizuj zaznaczenia menu:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case 555:
                exit(0);
                break;

            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_TIMER:
    {
        winda.ruch(kolejka);
        InvalidateRect(hWnd, NULL, TRUE);

    }
    break;
    case WM_PAINT:
        {
            
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Tutaj dodaj kod rysujący używający elementu hdc...
        // create memory DC and memory bitmap where we shall do our drawing

        HDC memDC = CreateCompatibleDC(hdc);

        // get window's client rectangle. We need this for bitmap creation.
        RECT rcClientRect;
        GetClientRect(hWnd, &rcClientRect);

        // now we can create bitmap where we shall do our drawing
        HBITMAP bmp = CreateCompatibleBitmap(hdc,
            rcClientRect.right - rcClientRect.left,
            rcClientRect.bottom - rcClientRect.top);

        // we need to save original bitmap, and select it back when we are done,
        // in order to avoid GDI leaks!
        HBITMAP oldBmp = (HBITMAP)SelectObject(memDC, bmp);
        FillRect(memDC, &rcClientRect, (HBRUSH)(COLOR_WINDOW + 1));
        Rysuj(memDC);

        BitBlt(hdc, 0, 0, rcClientRect.right - rcClientRect.left,
            rcClientRect.bottom - rcClientRect.top, memDC, 0, 0, SRCCOPY);

        // all done, now we need to cleanup
        SelectObject(memDC, oldBmp); // select back original bitmap
        DeleteObject(bmp); // delete bitmap since it is no longer required
        DeleteDC(memDC);   // delete memory DC since it is no longer required
        EndPaint(hWnd, &ps);
            
        }
        break;
    case WM_ERASEBKGND:
        return 1;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

//l = lewo, p = prawo, g = góra, d = dół

const Point lg(225, 500);
const Point ld(225, 625);
const Point pg(450, 500);
const Point pd(450, 625);

void Rysuj(HDC HDC) {
    int xxx = winda.y;
    Graphics grafika(HDC);
    Pen windak(Color(255, 0, 0, 0), 7);
    Pen pietrok(Color(255, 125, 200, 0), 6);

    grafika.DrawLine(&pietrok, 0, 125, 225, 125);
    grafika.DrawLine(&pietrok, 450, 250, 675, 250);
    grafika.DrawLine(&pietrok, 0, 375, 225, 375);
    grafika.DrawLine(&pietrok, 450, 500, 675, 500);
    grafika.DrawLine(&pietrok, 0, 625, 225, 625);

    grafika.DrawLine(&windak, ld.X, ld.Y - xxx, lg.X, lg.Y - xxx);
    grafika.DrawLine(&windak, lg.X, lg.Y - xxx, pg.X, pg.Y - xxx);
    grafika.DrawLine(&windak, pg.X, pg.Y - xxx, pd.X, pd.Y - xxx);
    grafika.DrawLine(&windak, pd.X, pd.Y - xxx, ld.X, ld.Y - xxx);

    Bitmap ludzik(L"ludzik.png");
    for (int i = 0; i < 5; i++)
    {
        for (auto ludek : kolejka[i])
        {
            Rect kontener(ludek.x, ludek.y, ludzik.GetWidth() / 7, ludzik.GetHeight() / 6);
            grafika.DrawImage(&ludzik, kontener);
        }
    }
    for (auto ludek : winda.osobywwindzie)
    {
        Rect kontener(ludek.x, ludek.y, ludzik.GetWidth() / 7, ludzik.GetHeight() / 6);
        grafika.DrawImage(&ludzik, kontener);
    }
};




// Procedura obsługi komunikatów dla okna informacji o programie.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

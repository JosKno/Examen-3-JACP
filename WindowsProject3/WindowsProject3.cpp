#include "framework.h"
#include "WindowsProject3.h"

#include <fstream>
#include <CommDlg.h>
using namespace std;

#define MAX_LOADSTRING 100


struct nodo_alumnos {
    int matricula;
    char nombre[20];
    char fecha[20];
    char hora[20];
    char lugar[20];
    int total, cantidad;
    nodo_alumnos* ant;
    nodo_alumnos* sig;
};
nodo_alumnos* aux = 0, * lista_alumnos = 0, * ultimo_alumno = 0, * nuevo_alumno = 0;


char arch_alumnos[] = "alumnos2.dat";
char mensaje[50] = { 0 };
char nombrearchivo[MAX_PATH] = { 0 };
bool _login = false;


HINSTANCE hInst;                                
WCHAR szTitle[MAX_LOADSTRING];                  
WCHAR szWindowClass[MAX_LOADSTRING];           


ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Comprar(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Lista(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    EVENTOS(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Login(HWND, UINT, WPARAM, LPARAM);


void leer_archivo(char* archivo);
void escribir_archivo(char* archivo);
void agregar_alumno(nodo_alumnos* aux);
void eliminar_alumno(nodo_alumnos* aux);
nodo_alumnos* BuscarAlumno(int matr);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT3, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT3));

    // leer el archivo
    leer_archivo(arch_alumnos);

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    
    // guardar el archivo con cada nodo de la lista ligada
    escribir_archivo(arch_alumnos);

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT3));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT3);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case ID_FILE_COMPRAR:
            if (_login) {
                DialogBox(hInst, MAKEINTRESOURCE(IDD_FORMVIEW), hWnd, Comprar);
            }
            else
            {
                MessageBox(0, "Debe conectarse primero", "ERROR",
                    MB_ICONEXCLAMATION + MB_OK);
            }
            break;
        case ID_FILE_HISTORIAL:
            if (_login) {
                DialogBox(
                    hInst,
                    MAKEINTRESOURCE(IDD_LISTA_ALUMNOS),
                    hWnd,
                    Lista);
            }
            else
            {
                MessageBox(0, "Debe conectarse primero", "ERROR",
                    MB_ICONEXCLAMATION + MB_OK);
            }
            break;
        case ID_FILE_EVENTOS:
            if (_login) {
                DialogBox(
                    hInst,
                    MAKEINTRESOURCE(IDD_EVENTOS),

                    hWnd,
                    EVENTOS);
            }
            else
            {
                MessageBox(0, "Debe conectarse primero", "ERROR",
                    MB_ICONEXCLAMATION + MB_OK);
            }

            break;
        case ID_FILE_LOGIN:

            DialogBox(
                hInst,
                MAKEINTRESOURCE(IDD_FORM_LOGIN),
                hWnd,
                Login);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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

// Message handler for about box.
INT_PTR CALLBACK Comprar(
            HWND hDlg, 
            UINT message, 
            WPARAM wParam, 
            LPARAM lParam)
{
    static HWND HWmat;
    static HWND HWcanti;
    static HWND HWtot;
    static HWND HWfe;
    static HWND HWlu;
    static HWND HWho;
    static HWND HWcan;
    static HWND HWpre;
    static HWND HWnombre;
    static HWND texto;
    static HWND texto2;
    static HWND texto3;
    static HWND texto4;
    static HWND evento;
    static HWND descuento;

    nodo_alumnos alumno_temp;
    char matricula[10] = { 0 };
    char cantidad[10] = { 0 };
    char total[10] = { 0 };
    char precio[10] = { 0 };
    char ev[10] = { 0 };
    char des[10] = { 0 };
    double can, sub, to, iva, pre;
    double dsub, dto, diva;

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)"BTS");
        SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)"Twice");
        SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)"NewJeans");
        SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)"TXT");
        SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)"BlackPink");
        SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)"Enhypen");

        SendDlgItemMessage(hDlg, IDC_COMBO2, CB_ADDSTRING, 0, (LPARAM)"10%");
        SendDlgItemMessage(hDlg, IDC_COMBO2, CB_ADDSTRING, 0, (LPARAM)"2x1");
        SendDlgItemMessage(hDlg, IDC_COMBO2, CB_ADDSTRING, 0, (LPARAM)"NO");
        
        HWcan =  GetDlgItem(hDlg, IDC_EDIT1);
        SendMessage(HWcan, WM_SETTEXT, 0, (LPARAM)"captura la cantidad");
        return (INT_PTR)TRUE;

    case WM_COMMAND:

        if (LOWORD(wParam) == IDC_CONF
            && HIWORD(wParam) == BN_CLICKED)
        {
            evento = GetDlgItem(hDlg, IDC_COMBO1);
            SendMessage(evento, WM_GETTEXT, 20, (LPARAM)ev);

            if ((strcmp(ev, "BTS") == 0)) {
                texto = GetDlgItem(hDlg, IDC_STATIC1);
                SetWindowText(texto, "Foro Sol");
                texto2 = GetDlgItem(hDlg, IDC_STATIC2);
                SetWindowText(texto2, "30/11/2023");
                texto3 = GetDlgItem(hDlg, IDC_STATIC3);
                SetWindowText(texto3, "19:00");
                texto4 = GetDlgItem(hDlg, IDC_STATIC4);
                SetWindowText(texto4, "1000");

            }
            if ((strcmp(ev, "Twice") == 0)) {
                texto = GetDlgItem(hDlg, IDC_STATIC1);
                SetWindowText(texto, "Foro Sol");
                texto2 = GetDlgItem(hDlg, IDC_STATIC2);
                SetWindowText(texto2, "06/02/2024");
                texto3 = GetDlgItem(hDlg, IDC_STATIC3);
                SetWindowText(texto3, "21:00");
                texto4 = GetDlgItem(hDlg, IDC_STATIC4);
                SetWindowText(texto4, "800");

            }
            if ((strcmp(ev, "TXT") == 0)) {
                texto = GetDlgItem(hDlg, IDC_STATIC1);
                SetWindowText(texto, "Palacio de los Deportes");
                texto2 = GetDlgItem(hDlg, IDC_STATIC2);
                SetWindowText(texto2, "31/10/2024");
                texto3 = GetDlgItem(hDlg, IDC_STATIC3);
                SetWindowText(texto3, "19:00");
                texto4 = GetDlgItem(hDlg, IDC_STATIC4);
                SetWindowText(texto4, "750");

            }
            if ((strcmp(ev, "NewJeans") == 0)) {
                texto = GetDlgItem(hDlg, IDC_STATIC1);
                SetWindowText(texto, "Palacio de los Deportes");
                texto2 = GetDlgItem(hDlg, IDC_STATIC2);
                SetWindowText(texto2, "31/11/2023");
                texto3 = GetDlgItem(hDlg, IDC_STATIC3);
                SetWindowText(texto3, "21:00");
                texto4 = GetDlgItem(hDlg, IDC_STATIC4);
                SetWindowText(texto4, "900");

            }
            if ((strcmp(ev, "BlackPink") == 0)) {
                texto = GetDlgItem(hDlg, IDC_STATIC1);
                SetWindowText(texto, "Foro Sol");
                texto2 = GetDlgItem(hDlg, IDC_STATIC2);
                SetWindowText(texto2, "30/03/2024");
                texto3 = GetDlgItem(hDlg, IDC_STATIC3);
                SetWindowText(texto3, "19:00");
                texto4 = GetDlgItem(hDlg, IDC_STATIC4);
                SetWindowText(texto4, "950");

            }
            if ((strcmp(ev, "Enhypen") == 0)) {
                texto = GetDlgItem(hDlg, IDC_STATIC1);
                SetWindowText(texto, "Palacio de los Deportes");
                texto2 = GetDlgItem(hDlg, IDC_STATIC2);
                SetWindowText(texto2, "10/08/2024");
                texto3 = GetDlgItem(hDlg, IDC_STATIC3);
                SetWindowText(texto3, "21:00");
                texto4 = GetDlgItem(hDlg, IDC_STATIC4);
                SetWindowText(texto4, "650");

            }
            return (INT_PTR)TRUE;
        }

        if (LOWORD(wParam) == IDC_BUTTON1
            && HIWORD(wParam) == BN_CLICKED)
        {
            HWcan = GetDlgItem(hDlg, IDC_EDIT1);
            SendMessage(HWcan, WM_GETTEXT, 10, (LPARAM)cantidad);
            HWpre = GetDlgItem(hDlg, IDC_STATIC4);
            SendMessage(HWpre, WM_GETTEXT, 10, (LPARAM)precio);
            can = atoi(cantidad);
            pre = atoi(precio);
            sub = can * pre;
            iva = sub * .16;
            to = sub + iva;
            SetDlgItemInt(hDlg, IDC_STATIC5, sub, TRUE);
            SetDlgItemInt(hDlg, IDC_STATIC6, iva, TRUE);
            SetDlgItemInt(hDlg, IDC_STATIC7, to, TRUE);
            return (INT_PTR)TRUE;
        }

        if (LOWORD(wParam) == IDC_BUTTON2
            && HIWORD(wParam) == BN_CLICKED)
        {
            SendMessage(HWcan, WM_GETTEXT, 10, (LPARAM)cantidad);
            HWpre = GetDlgItem(hDlg, IDC_STATIC4);
            SendMessage(HWpre, WM_GETTEXT, 10, (LPARAM)precio);
            can = atoi(cantidad);
            pre = atoi(precio);
            sub = can * pre;
            iva = sub * .16;
            to = sub + iva;
            descuento = GetDlgItem(hDlg, IDC_COMBO2);
            SendMessage(descuento, WM_GETTEXT, 20, (LPARAM)des);
            if ((strcmp(des, "2x1") == 0)) {
                HWcan = GetDlgItem(hDlg, IDC_EDIT1);
                
                dsub = sub * .5;
                diva = dsub * .16;
                dto = dsub + diva;
                SetDlgItemInt(hDlg, IDC_STATIC5, dsub, TRUE);
                SetDlgItemInt(hDlg, IDC_STATIC6, diva, TRUE);
                SetDlgItemInt(hDlg, IDC_STATIC7, dto, TRUE);
            }
            if ((strcmp(des, "10%") == 0)) {
                dsub = sub * .9;
                diva = dsub * .16;
                dto = dsub + diva;
                SetDlgItemInt(hDlg, IDC_STATIC5, dsub, TRUE);
                SetDlgItemInt(hDlg, IDC_STATIC6, diva, TRUE);
                SetDlgItemInt(hDlg, IDC_STATIC7, dto, TRUE);
            }
           
            return (INT_PTR)TRUE;
        }

        if (LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }

        


        // aquí va el código que quiero que se ejecute
        // cuando el usuario le da clic al botón

                                // Id del botón
        if (LOWORD(wParam) == IDC_COMPRAR
            && HIWORD(wParam) == BN_CLICKED)
        {            
            // Obtener el Windows Handler de cada objeto
            HWcan = GetDlgItem(hDlg, IDC_EDIT1);
            HWfe = GetDlgItem(hDlg, IDC_STATIC2);
            HWlu = GetDlgItem(hDlg, IDC_STATIC1);
            HWho = GetDlgItem(hDlg, IDC_STATIC3);
            HWmat = GetDlgItem(hDlg, IDC_EDIT2);
            HWnombre = GetDlgItem(hDlg, IDC_COMBO1);
            HWtot = GetDlgItem(hDlg, IDC_STATIC7);
            // mandano un mensaje
            // para leer lo que el usuario capturó 
            // en las cajas de texto: Edit Control
            // se pasa a las variables
            SendMessage(HWcan, WM_GETTEXT, 10, (LPARAM)matricula);
            SendMessage(HWtot, WM_GETTEXT, 10, (LPARAM)total);
            SendMessage(HWcanti, WM_GETTEXT, 10, (LPARAM)cantidad);
            SendMessage(HWnombre, WM_GETTEXT, 20, (LPARAM)alumno_temp.nombre);
            SendMessage(HWfe, WM_GETTEXT, 20, (LPARAM)alumno_temp.fecha);
            SendMessage(HWlu, WM_GETTEXT, 20, (LPARAM)alumno_temp.lugar);
            SendMessage(HWho, WM_GETTEXT, 20, (LPARAM)alumno_temp.hora);

            // convirtiendo de char a integer
            alumno_temp.matricula = atoi(matricula);
            alumno_temp.cantidad = atoi(cantidad);
            alumno_temp.total = atoi(total);

            // Crear nuevo nodo
            nodo_alumnos* nuevo = new nodo_alumnos;
            nuevo->matricula = alumno_temp.matricula;

            strcpy(nuevo->nombre, alumno_temp.nombre);


            // AGREGAR NODO
            agregar_alumno(nuevo);
            // prepara el mensaje
            strcpy(mensaje, "Compra: ");
            strcat(mensaje, alumno_temp.nombre);
            strcat(mensaje, ", ha sido efectuada");

            MessageBox(0, mensaje, "COMPRAR",
                MB_ICONINFORMATION + MB_OK);
            //limpiar las cajitas de texto
            SendMessage(HWcan, WM_SETTEXT, 0, (LPARAM)"");
            SendMessage(HWmat, WM_SETTEXT, 0, (LPARAM)"");
            SendMessage(HWnombre, WM_SETTEXT, 0, (LPARAM)"");


            return (INT_PTR)TRUE;
        }


        break;
    }
    return (INT_PTR)FALSE;
}


// Message handler for about box.
INT_PTR CALLBACK Lista(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    nodo_alumnos* temp = 0;
    int idx = 0;
    int matricula = 0;
    char s_matr[10] = { 0 };
    char nombre[20] = { 0 };
    char lugar[20] = { 0 };
    char fecha[20] = { 0 };
    char hora[20] = { 0 };
   

    switch (message)
    {
    case WM_INITDIALOG:
       

        SendDlgItemMessage(hDlg, IDC_LIST2, LB_RESETCONTENT, 0, 0);
        SendDlgItemMessage(hDlg, IDC_CBO_ALUMNOS, CB_RESETCONTENT, 0, 0);
        //leer la lista ligada, nodo por nodo
        idx = 0;
        aux = lista_alumnos;
        while (aux != 0)
        {
            // por cada nodo, agregar al LISTBIOX             ​
            SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 
                                0, 
                                (LPARAM)aux->nombre);
            //IDC_CBO_ALUMNOS
            SendDlgItemMessage(hDlg, IDC_CBO_ALUMNOS, CB_ADDSTRING,
                                0,
                                (LPARAM)aux->nombre);
            // asociar la matricula como dato adicional del ListBox
            SendDlgItemMessage(hDlg, IDC_LIST2, LB_SETITEMDATA,
                idx,                
                aux->matricula);
            SendDlgItemMessage(hDlg, IDC_CBO_ALUMNOS, CB_SETITEMDATA,
                idx,
                aux->matricula);
            idx++;
            aux = aux->sig;
        }



        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        if (LOWORD(wParam) == IDC_OK)
        {
            idx = SendDlgItemMessage(hDlg, IDC_CBO_ALUMNOS, CB_GETCURSEL, 0, 0);
            SendDlgItemMessage(hDlg, IDC_CBO_ALUMNOS, CB_GETLBTEXT, idx, (LPARAM)nombre);
            matricula = SendDlgItemMessage(hDlg, IDC_CBO_ALUMNOS, CB_GETITEMDATA, idx, 0);

            _itoa(matricula, s_matr, 10);
            strcat(nombre, " - ");
            strcat(nombre, s_matr);
            strcat(nombre, " Whisper");
            MessageBox(0, nombre, "LISTA COMPRAS", MB_ICONINFORMATION + MB_OK);

            strcpy(nombre, "");
            // tengo matricula
            temp = BuscarAlumno(matricula);
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// Message handler for about box.
INT_PTR CALLBACK EVENTOS(
            HWND hDlg, UINT message, 
            WPARAM wParam, LPARAM lParam)
{
    static HWND texto;
    static HWND texto2;
    static HWND texto3;
    static HWND texto4;
    static HWND evento;

    char ev[10] = {};
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)"BTS");
        SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)"Twice");
        SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)"NewJeans");
        SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)"TXT");
        SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)"BlackPink");
        SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)"Enhypen");

        SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)"BTS");
        SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)"Twice");
        SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)"NewJeans");
        SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)"TXT");
        SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)"BlackPink");
        SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)"Enhypen");
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BUTTON1
            && HIWORD(wParam) == BN_CLICKED)
        {
            evento = GetDlgItem(hDlg, IDC_COMBO1);
            SendMessage(evento, WM_GETTEXT, 20, (LPARAM)ev);

            if ((strcmp(ev, "BTS") == 0)) {
                texto = GetDlgItem(hDlg, IDC_EDIT1);
                SetWindowText(texto, "Foro Sol");
                texto2 = GetDlgItem(hDlg, IDC_EDIT2);
                SetWindowText(texto2, "30/11/2023");
                texto3 = GetDlgItem(hDlg, IDC_EDIT3);
                SetWindowText(texto3, "19:00");
                texto4 = GetDlgItem(hDlg, IDC_EDIT4);
                SetWindowText(texto4, "1000");
               
            }
            if ((strcmp(ev, "Twice") == 0)) {
                texto = GetDlgItem(hDlg, IDC_EDIT1);
                SetWindowText(texto, "Foro Sol");
                texto2 = GetDlgItem(hDlg, IDC_EDIT2);
                SetWindowText(texto2, "06/02/2024");
                texto3 = GetDlgItem(hDlg, IDC_EDIT3);
                SetWindowText(texto3, "21:00");
                texto4 = GetDlgItem(hDlg, IDC_EDIT4);
                SetWindowText(texto4, "800");

            }
            if ((strcmp(ev, "TXT") == 0)) {
                texto = GetDlgItem(hDlg, IDC_EDIT1);
                SetWindowText(texto, "Palacio de los Deportes");
                texto2 = GetDlgItem(hDlg, IDC_EDIT2);
                SetWindowText(texto2, "31/10/2024");
                texto3 = GetDlgItem(hDlg, IDC_EDIT3);
                SetWindowText(texto3, "19:00");
                texto4 = GetDlgItem(hDlg, IDC_EDIT4);
                SetWindowText(texto4, "750");

            }
            if ((strcmp(ev, "NewJeans") == 0)) {
                texto = GetDlgItem(hDlg, IDC_EDIT1);
                SetWindowText(texto, "Palacio de los Deportes");
                texto2 = GetDlgItem(hDlg, IDC_EDIT2);
                SetWindowText(texto2, "31/11/2023");
                texto3 = GetDlgItem(hDlg, IDC_EDIT3);
                SetWindowText(texto3, "21:00");
                texto4 = GetDlgItem(hDlg, IDC_EDIT4);
                SetWindowText(texto4, "900");

            }
            if ((strcmp(ev, "BlackPink") == 0)) {
                texto = GetDlgItem(hDlg, IDC_EDIT1);
                SetWindowText(texto, "Foro Sol");
                texto2 = GetDlgItem(hDlg, IDC_EDIT2);
                SetWindowText(texto2, "30/03/2024");
                texto3 = GetDlgItem(hDlg, IDC_EDIT3);
                SetWindowText(texto3, "19:00");
                texto4 = GetDlgItem(hDlg, IDC_EDIT4);
                SetWindowText(texto4, "950");

            }
            if ((strcmp(ev, "Enhypen") == 0)) {
                texto = GetDlgItem(hDlg, IDC_EDIT1);
                SetWindowText(texto, "Palacio de los Deportes");
                texto2 = GetDlgItem(hDlg, IDC_EDIT2);
                SetWindowText(texto2, "10/08/2024");
                texto3 = GetDlgItem(hDlg, IDC_EDIT3);
                SetWindowText(texto3, "21:00");
                texto4 = GetDlgItem(hDlg, IDC_EDIT4);
                SetWindowText(texto4, "650");

            }
            return (INT_PTR)TRUE;

        }
        if (LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;

    }
    

   
    
    return (INT_PTR)FALSE;
}

// Message handler for Login Form. Oct-27
INT_PTR CALLBACK Login(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND HWuser;
    static HWND HWpass;
    char user[10] = {};
    char pass[20] = {};
    char texto[100] = {};
    char u[10] = "Whisper";
    char p[20] = "contrasena";

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
        if (LOWORD(wParam) == IDC_OK && HIWORD(wParam) == BN_CLICKED)
        {
       
            HWuser = GetDlgItem(hDlg, IDC_USER);
            HWpass = GetDlgItem(hDlg, IDC_PASS);
            SendMessage(HWuser, WM_GETTEXT, 20, (LPARAM)user);
            SendMessage(HWpass, WM_GETTEXT, 20, (LPARAM)pass);
            if ((strcmp(user, u) == 0) && (strcmp(pass, p) == 0)) {
                _login = true;
            }
            if (_login) {
                strcpy(texto, "Bienvenido al sistema de tickets: ");
                strcat(texto, user);
                MessageBox(0, texto, "MENSAJE",
                    MB_ICONINFORMATION + MB_OK);
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            else {
                MessageBox(0, "Usuario no existe y/o contraseña incorrecta", "MENSAJE",
                    MB_ICONERROR + MB_OK);
                return (INT_PTR)TRUE;
            }
        }
        if (LOWORD(wParam) == IDC_PSW && HIWORD(wParam) == BN_CLICKED)
        {
            SendMessage(hDlg, DM_SETDEFID, (WPARAM)IDC_PASS, (LPARAM)0);
            //SendMessage(NULL, EM_GETPASSWORDCHAR, IDC_PASS
        }
        if (LOWORD(wParam) == IDC_CERRAR && HIWORD(wParam) == BN_CLICKED)
        {
            // Leer el contenido de los EditControl
            // Buscar en el Archivo - Usuarios
            // si coincide, hacer true a la variable _login
            _login = false;
            MessageBox(0, "Usuario no existe y/o contraseña incorrecta", "MENSAJE",
                MB_ICONERROR + MB_OK);
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

//Definición de funciones
void leer_archivo(char* archivo) {
    ifstream arch_alumnos;
    arch_alumnos.open(archivo, ios::binary);
    if (arch_alumnos.is_open())
    {
        // leer el archivo, hasta el fin 
        nuevo_alumno = new nodo_alumnos;
        arch_alumnos.read((char*)nuevo_alumno, sizeof(nodo_alumnos));
        while (!arch_alumnos.eof())
        {
            aux = new nodo_alumnos;
            aux = nuevo_alumno;

            agregar_alumno(aux);

            nuevo_alumno = new nodo_alumnos;
            arch_alumnos.read((char*)nuevo_alumno, sizeof(nodo_alumnos));
        }
        arch_alumnos.close();
        //printf("archivo leído, lista ligada de alumnos creada\n");
        //system("pause");
    }
}

void escribir_archivo(char* archivo) {
    ofstream archi;
    archi.open(archivo, ios::trunc | ios::binary);
    if (archi.is_open())
    {
        // recorrer toda la lista ligada, nodo x nodo
        aux = lista_alumnos;
        while (aux != 0)
        {
            //guardar en el archivo cada nodo
            archi.write((char*)aux, sizeof(nodo_alumnos));

            //leer el siguiente nodo
            aux = aux->sig;
        }
        archi.close();
        //printf("Archivo guardado\n\n");
    }
}

void agregar_alumno(nodo_alumnos* aux) {
    aux->ant = 0;
    aux->sig = 0;
    if (lista_alumnos == 0) { // está vacía
        lista_alumnos = aux;
    }
    else { // no está vacía
        ultimo_alumno->sig = aux;
        aux->ant = ultimo_alumno;
    }
    ultimo_alumno = aux;

}

void eliminar_alumno(nodo_alumnos* aux) {
    // si el nodo a borrar es el primero
    if (aux == lista_alumnos) {
        lista_alumnos = aux->sig;
        lista_alumnos->ant = 0;
    }
    else {
        // si el nodo a borrar es el ultimo
        if (aux == ultimo_alumno)
        {
            ultimo_alumno = aux->ant;
            ultimo_alumno->sig = 0;
        }
        else
        {
            // modificar ant y sig de los nodos vecinos
            nodo_alumnos* prev = aux->ant;
            nodo_alumnos* next = aux->sig;
            prev->sig = aux->sig;
            if (next != 0) {
                next->ant = aux->ant;
            }
        }

    }
    delete aux;
}

nodo_alumnos* BuscarAlumno(int matr) {
    nodo_alumnos* alum = 0;
    aux = 0;
    aux = lista_alumnos;
    while (aux != 0)
    {
        if (aux->matricula == matr) {
            alum = aux;
            break;
        }
        aux = aux->sig;
    }
    return alum;
}



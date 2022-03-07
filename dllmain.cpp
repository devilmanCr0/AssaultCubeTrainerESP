// dllmain.cpp : Defines the entry point for the DLL application.
#include "mMem.h"
#include "sig32.h"
#include "hooking.h"
#include "glDraw.h"
#include "glText.h"
#include "geom.h"
/*
Might need to set up static addresses or type defs here like we did in our previous hack
stay tuned
*/
typedef BOOL(__stdcall* twglSwapBuffers)(HDC hDc);

uintptr_t moduleBase;

std::vector<Ent*> EntList;
Ent* localPlayer;
DWORD matrix;
int teamSize;

bool SBSwitch{ 0 };
bool bSwitch{ 0 };
bool bESPSwitch{ 0 };
bool bESP{ 0 };
bool bEnd{ 0 };
bool bESPSwitchInitiate{ 0 };


twglSwapBuffers OwglSwapBufferGateway;

GL::Font glFont;
const int FONT_HEIGHT = 20;
const int FONT_WIDTH = 10;

const char* textexample = "PornhubFam";
const char* textexample2 = "IminvestingZAZA";

//need to check so that hdc doesnt change randomly among changing game modes in ur game
void Draw()
{
    HDC currentHDC = wglGetCurrentDC();

    if (!glFont.bBuilt || currentHDC != glFont.hdc)
    {
        glFont.Build(FONT_HEIGHT);

    }

    GL::SetupOrtho();

    glGetIntegerv(GL_VIEWPORT, viewport);

    for (int i{ 0 }; i < teamSize - 1; i++)
    {
        Vector3L center = EntList[i]->HeadPos;
        center.z = center.z - EYE_HEIGHT + PLAYER_HEIGHT / 2;

        Vector3L screenCoords;
        if (WorldToScreen(center, screenCoords, (float*)matrix, viewport[2], viewport[3]))
        {
            GL::DrawESPBox(EntList[i], localPlayer, screenCoords);
        }
    }

    GL::RestoreGL();


}


DWORD entityAddy{};
DWORD jumpBackAddy{};
DWORD entPtr{};
DWORD botVtable{ 0x54AFEC };
DWORD playerVtable{ 0x54B014 };


DWORD tranversedEnt{};
DWORD jumpBackAddyTwo{};
int oldSize{};



bool isValidEnt(Ent* ent)
{
    if (ent)
    {
        if (ent->vTable == botVtable || ent->vTable == playerVtable)
        {
            return true;
        }


    }

    return false;
}


__declspec(naked) void entListCapture()
{
    _asm
    {
        mov[esi + 65], al
        mov[tranversedEnt], esi
        mov eax, [esi]
        jmp[jumpBackAddyTwo]

    }
}




//hacked swap buffer
BOOL __stdcall hkwglSwapBuffers(HDC hDc)
{
    ///////////////////////////////////////TheSwitches

    if (GetAsyncKeyState(VK_END) & 1)
    {
        bESP = false;
        bEnd = true;
    }

 

    if (GetAsyncKeyState(VK_HOME) & 1)
    {
        bESP = !bESP;
    }


    ////////////////////////////////////////

    if (bESP && bESPSwitchInitiate)
    {
        Draw();
    }

    teamSize = *(int*)(moduleBase + 0x187C18);
    localPlayer = *(Ent**)(moduleBase + 0x187C0C);

    ////////////////////////////////////////Running the switches


    return OwglSwapBufferGateway(hDc);
}






DWORD WINAPI ConsoleMainFrame(HMODULE hModule)
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);



    std::cout << "\t\t\t Hello Gooner\n";
    std::cout << "\t\t\t Welcome to the all New Exclusive cum Cheats\n";
    std::cout << "\t\t Your Controls are the following \n";
    std::cout << "\t\t HOME KEY - ESP\n"; //4th row
    std::cout << "\t\t END KEY - EJECT HACK\n";

    moduleBase = (uintptr_t)GetModuleHandle(L"ac_client.exe");
    matrix = moduleBase + 0x17AFE0;
    localPlayer = nullptr;
   
    //create an upgraded version of our esp to finalize our findings
    teamSize = 0;
    

    int byteSizeTwo{ 5 };
    DWORD hookAddyTwo{ moduleBase + 0xC0C31 }; //address for EntList
    jumpBackAddyTwo = hookAddyTwo + byteSizeTwo;
    //size of bytes to be overwritten is 5 MINIMUM IS 5 we're in the clear
    hook((void*)hookAddyTwo, entListCapture, byteSizeTwo);


    //hooking our esp
    


    Hooking SwapBuffersHook("wglSwapBuffers", "opengl32.dll" , (BYTE*)hkwglSwapBuffers, (BYTE*)&OwglSwapBufferGateway, 5); 

   
    SwapBuffersHook.Enable();


    while (1)
    {
        if (bEnd)
        {
            break;
        }
        if (bESP)
        {
            if (teamSize != oldSize)
            {
                bESPSwitch = 1;
                EntList.clear();
                oldSize = teamSize;
                bESPSwitchInitiate = 0;
            }
            if (bESPSwitch)
            {
                while (EntList.size() != teamSize - 1)
                {
                    if ((*(DWORD*)tranversedEnt == playerVtable || *(DWORD*)tranversedEnt == botVtable))
                    {
                        if ((Ent*)tranversedEnt != localPlayer)
                        {
                            if (EntList.size() == 0)
                            {
                                EntList.push_back((Ent*)tranversedEnt);
                            }
                            else if ((DWORD*)tranversedEnt != (DWORD*)EntList[EntList.size() - 1])
                            {
                                EntList.push_back((Ent*)tranversedEnt);
                            }
                        }
                    }

                }
                bESPSwitch = 0;
                bESPSwitchInitiate = 1;
            }
            

        }
    }


    memoryAllocate(byteSizeTwo, (BYTE*)moduleBase + 0xC0C31, (BYTE*)"\x88\x46\x65\x8B\x06");
    SwapBuffersHook.Disable();








    std::cout << "\t\t\t\tHACK EJECTED - THANK YOU";
    Sleep(1000);


    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}





BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ConsoleMainFrame, hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


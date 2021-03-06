#include <stdio.h>
#include <windows.h>
#include "SMX.h"
#include <memory>
#include <string>
using namespace std;

class InputSample
{
public:
    InputSample()
    {
        // Set a logging callback.  This can be called before SMX_Start.
        // SMX_SetLogCallback( SMXLogCallback );

        // Start scanning.  The update callback will be called when devices connect or
        // disconnect or panels are pressed or released.  This callback will be called
        // from a thread.
        SMX_Start( SMXStateChangedCallback, this );
    }

    static void SMXStateChangedCallback(int pad, SMXUpdateCallbackReason reason, void *pUser)
    {
        InputSample *pSelf = (InputSample *) pUser;
        pSelf->SMXStateChanged( pad, reason );
    }

    static void SMXLogCallback(const char *log)
    {
        printf("-> %s\n", log);
    }

    void SMXStateChanged(int pad, SMXUpdateCallbackReason reason)
    {
        printf("Device %i state changed: %04x\n", pad, SMX_GetInputState(pad));

    }

    int iPanelToLight = 0;
    void SetLights()
    {
        string sLightsData;
        auto addColor = [&sLightsData](uint8_t r, uint8_t g, uint8_t b)
        {
            sLightsData.append( 1, r );
            sLightsData.append( 1, g );
            sLightsData.append( 1, b );
        };
        for( int iPad = 0; iPad < 2; ++iPad )
        {
            for( int iPanel = 0; iPanel < 9; ++iPanel )
            {
                bool bLight = iPanel == iPanelToLight && iPad == 0;
                if( !bLight )
                {
                    // We're not lighting this panel, so append black for the 4x4 and 3x3 lights.
                    for( int iLED = 0; iLED < 25; ++iLED )
                        addColor( 0, 0, 0 );
                    continue;
                }

                // Append light data for the outer 4x4 grid of lights.
                addColor( 0xFF, 0, 0 );
                addColor( 0xFF, 0, 0 );
                addColor( 0xFF, 0, 0 );
                addColor( 0xFF, 0, 0 );
                addColor( 0, 0xFF, 0 );
                addColor( 0, 0xFF, 0 );
                addColor( 0, 0xFF, 0 );
                addColor( 0, 0xFF, 0 );
                addColor( 0, 0, 0xFF );
                addColor( 0, 0, 0xFF );
                addColor( 0, 0, 0xFF );
                addColor( 0, 0, 0xFF );
                addColor( 0xFF, 0xFF, 0 );
                addColor( 0xFF, 0xFF, 0 );
                addColor( 0xFF, 0xFF, 0 );
                addColor( 0xFF, 0xFF, 0 );

                // Append light data for the inner 3x3 grid of lights, if present.  These
                // are ignored if the platform doesn't have them.
                addColor( 0xFF, 0, 0 );
                addColor( 0xFF, 0, 0 );
                addColor( 0xFF, 0, 0 );
                addColor( 0, 0xFF, 0 );
                addColor( 0, 0xFF, 0 );
                addColor( 0, 0xFF, 0 );
                addColor( 0, 0, 0xFF );
                addColor( 0, 0, 0xFF );
                addColor( 0, 0, 0xFF );
            }
        }

        SMX_SetLights2( sLightsData.data(), sLightsData.size() );
    }
};

int main()
{
    InputSample demo;

    // Loop forever for this sample.
    while(1)
    {
        Sleep(500);
        demo.SetLights();
    }

    return 0;
}


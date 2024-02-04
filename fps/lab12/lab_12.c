#include <stdio.h>
#include "pbPlots.h"
#include "supportLib.h"
#define NUM_TAPS 64

//question 2 confirmation

int main(){

double ys[NUM_TAPS] = {0.0f};
double xs[NUM_TAPS] = {0.0f};

for (int i = 0; i < NUM_TAPS; i++)
    {   
        xs[i] = i;
        //printf("%f ",x[i]);
    }

//puts("\n");

int start, k;
// initialize our reference FIR impulse response
start = 4;
for(k=0; k <= 5; k++)
    {
        *(ys+start+k) = - 0.1f * (double)(k+1); //change  - ---> +
        *(ys+start+10-k) = *(ys+start+k);     
    }

start += 11;
for(k=0; k <= 16; k++)
    {
        *(ys+start+k) = 0.15f * (double)(k+1);
        *(ys+start+32-k) =  *(ys+start+k); //change + ---> -
    }

start += 33;
for(k=0; k < 11; k++)
    {
        *(ys+start+k) = *(ys+4+k);  //change + ---> +
    }

for (int i = 0; i < NUM_TAPS; i++)
    {
        //printf("%f ",hREF[i]);
    }


    //plot 

    _Bool success;
	StringReference *errorMessage;

	RGBABitmapImageReference *imageReference = CreateRGBABitmapImageReference();

	ScatterPlotSeries *series = GetDefaultScatterPlotSeriesSettings();
	series->xs = xs;
	series->xsLength = sizeof(xs)/sizeof(double);
	series->ys = ys;
	series->ysLength = sizeof(ys)/sizeof(double);
	series->linearInterpolation = true;
	series->lineType = L"solid";
	series->lineTypeLength = wcslen(series->lineType);
	series->lineThickness = 2;
	series->color = CreateRGBColor(0, 0, 1);


	ScatterPlotSettings *settings = GetDefaultScatterPlotSettings();
	settings->width = 600;
	settings->height = 400;
	settings->autoBoundaries = true;
	settings->autoPadding = true;
	settings->title = L"";
	settings->titleLength = wcslen(settings->title);
	settings->xLabel = L"";
	settings->xLabelLength = wcslen(settings->xLabel);
	settings->yLabel = L"";
	settings->yLabelLength = wcslen(settings->yLabel);
	ScatterPlotSeries *s [] = {series};
	settings->scatterPlotSeries = s;
	settings->scatterPlotSeriesLength = 1;

	errorMessage = (StringReference *)malloc(sizeof(StringReference));
	success = DrawScatterPlotFromSettings(imageReference, settings, errorMessage);

	if(success){
		size_t length;
		double *pngdata = ConvertToPNG(&length, imageReference->image);
		WriteToFile(pngdata, length, "original.png");
		DeleteImage(imageReference->image);
	}else{
		fprintf(stderr, "Error: ");
		for(int i = 0; i < errorMessage->stringLength; i++){
			fprintf(stderr, "%c", errorMessage->string[i]);
		}
		fprintf(stderr, "\n");
	}

	FreeAllocations();

	return success ? 0 : 1;

}
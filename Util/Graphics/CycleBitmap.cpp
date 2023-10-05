//------------------------------------------------------------------------------
//  Copyright : (c) 2009
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "CycleBitmap.h"

#include "LoadImage.h"
#include "../Core/File.h"
#include "../domXML/domXML.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Cycle
//------------------------------------------------------------------------------
CycleBitmap::Cycle::Cycle(int start, int end, int delay, bool fwd)
	:
	delay(delay),
	startColor(start),
	endColor(end),
	curDelay(delay),
	forward(fwd)
{}


//------------------------------------------------------------------------------
//	CycleBitmap
//------------------------------------------------------------------------------
CycleBitmap::CycleBitmap()
{
}


CycleBitmap::CycleBitmap(const char* image, const char* cycles)
{
	loadImage(image);
	loadCycles(cycles);
}


void CycleBitmap::update()
{
	RGBQUAD cycledColors[256];

	image.palette(0, 256, cycledColors);

	for (int i = 0; i < (int)cycles.size(); ++i)
	{
		Cycle& cycle = cycles[i];

		if (--cycle.curDelay <= 0)
		{
			cycle.curDelay = cycle.delay;

			if (cycle.forward)
			{
				const RGBQUAD backup = cycledColors[cycle.endColor];

				for (int j = cycle.endColor; j > cycle.startColor; --j)
				{
					cycledColors[j] = cycledColors[j - 1];
				}

				cycledColors[cycle.startColor] = backup;
			}
			else
			{
				const RGBQUAD backup = cycledColors[cycle.startColor];

				for (int j = cycle.startColor; j < cycle.endColor; ++j)
				{
					cycledColors[j] = cycledColors[j + 1];
				}

				cycledColors[cycle.endColor] = backup;
			}
		}
	}

	image.setPalette(0, 256, cycledColors);
}


void CycleBitmap::updatePalette()
{
	image.palette(0, 256, colors);
}


void CycleBitmap::reset()
{
	image.setPalette(0, 256, colors);
}


void CycleBitmap::loadImage(const char* fileName)
{
	Graphics::LoadImage(fileName, *this);
	image.palette(0, 256, colors);
}


void CycleBitmap::loadCycles(const char* fileName)
{
	auto xml = DomXML::getFirstNode(fileName);

	if (!xml.isValid())
	{
		MessageBox(
			NULL,
			"Invalid cycle file specified.",
			"Invalid File",
			MB_OK | MB_ICONEXCLAMATION);

		return;
	}

	cycles.clear();
	reset();

	const auto cycleList = xml.getNode("cycles");

	for (int i = 0; i < cycleList.size(); ++i)
	{
		const auto cycle = cycleList.getNode(i);

		const bool forward = cycle.attribute("dir") == "forward";
		int start, end, delay;

		const int count =
			sscanf_s(
				cycle.insideText(),
				"%d,%d,%d",
				&start,
				&end,
				&delay);

		if (count != 3)
		{
			continue;
		}

		cycles.push_back(Cycle(start, end, delay, forward));
	}
}


void CycleBitmap::loadPal(const char* fileName)
{
	if (fileName == NULL)
	{
		return;
	}

	File pal(fileName, File::eOption_Read_Binary);

	RGBQUAD newColors[256];

	if (!pal.read(newColors, sizeof(newColors)))
	{
		return;
	}

	memcpy(colors, newColors, sizeof(newColors));

	reset();
}


void CycleBitmap::removeCycle(int index)
{
	if (index < 0 || index >= (int)cycles.size())
	{
		return;
	}

	for (int i = index; i < (int)cycles.size() - 2; ++i)
	{
		cycles[i] = cycles[i + 1];
	}

	cycles.pop_back();

	reset();
}


void CycleBitmap::saveCycles(const char* fileName)
{
	if (fileName == NULL)
	{
		return;
	}

	File file(fileName, File::eOption_Write);

	fprintf(file, "<xml>\n\n");

	fprintf(file, "<cycles>\n");
	{
		for (int i = 0; i < (int)cycles.size(); ++i)
		{
			fprintf(
				file,
				"  <cycle dir=\"%s\">%d,%d,%d</cycle>\n",
				cycles[i].forward ? "forward" : "backward",
				cycles[i].startColor,
				cycles[i].endColor,
				cycles[i].delay);
		}
	}
	fprintf(file, "</cycles>\n\n");

	fprintf(file, "</xml>\n");
}


void CycleBitmap::savePal(const char* fileName)
{
	if (fileName == NULL)
	{
		return;
	}

	File pal(fileName, File::eOption_Write_Binary);

	pal.write(colors, sizeof(colors));
}
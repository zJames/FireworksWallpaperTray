//------------------------------------------------------------------------------
//  Copyright : (c) 2020
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "Fireworks.h"

#include "ParticleSystem.h"
#include "RandMath.h"
#include "Resource.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	External Variables
//------------------------------------------------------------------------------
extern std::vector<SharedImage> sImageFiles;
extern std::vector<RefString> sTexts;


//------------------------------------------------------------------------------
//	Local Types
//------------------------------------------------------------------------------
typedef std::list<Particle> ParticleList;


//------------------------------------------------------------------------------
//	Local Function Declarations
//------------------------------------------------------------------------------
void addFlares(ParticleList& toAdd, const Vector2<Fx32>& position);
void addSmallFlares(ParticleList& toAdd, const Vector2<Fx32>& position, const ARGB& color);
void addMessage(ParticleList& toAdd, const Vector2<Fx32>& position, RefString text);
void addFlag(ParticleList& toAdd, const Vector2<Fx32>& position);
void addImage(ParticleList& toAdd, const Vector2<Fx32>& position, Bitmap& image);


//------------------------------------------------------------------------------
//	Local Data
//------------------------------------------------------------------------------
const Vector2<Fx32> kGravity(0, Fx32(0.15f));

const ARGB kSparkleFlares[] =
{
	ARGB(255, 0, 0),
	ARGB(0, 255, 0),
	ARGB(0, 0, 255),
	ARGB(255, 255, 255),
	ARGB(255, 255, 0),
	ARGB(255, 0, 255),
	ARGB(0, 255, 255),
	ARGB(255, 127, 0),
	ARGB(255, 0, 127),
	ARGB(0, 255, 127),
	ARGB(127, 255, 0),
	ARGB(127, 0, 255),
	ARGB(0, 127, 255),
};

const int kSparkleFlareNum[] =
{
	8,
	8,
	8,
	8,
	16,
	16,
	16,
	16,
	32,
	32,
	64,
};

SharedImage sAlphabet;
SharedImage sFlag;


//------------------------------------------------------------------------------
//	Local Functions
//------------------------------------------------------------------------------
void decrease(ARGB& color)
{
	color.red = max(color.red - 32, 0);
	color.green = max(color.green - 32, 0);
	color.blue = max(color.blue - 32, 0);
}


//------------------------------------------------------------------------------
//	Firework Updates
//------------------------------------------------------------------------------
void messageUpdate(Particle* particle, ParticleList& toAdd)
{
	particle->velocity += kGravity / 2;
	particle->position += particle->velocity;

	if (particle->lifeTime < 5)
	{
		decrease(particle->color);
	}
}

void flareUpdate(Particle* particle, ParticleList& toAdd)
{
	particle->velocity += kGravity / 2;
	particle->position += particle->velocity;

	if (particle->lifeTime < 5)
	{
		decrease(particle->color);
	}
}

void superFlareUpdate(Particle* particle, ParticleList& toAdd)
{
	flareUpdate(particle, toAdd);

	if (particle->lifeTime == 0)
	{
		addSmallFlares(toAdd, particle->position, kSparkleFlares[particle->varA]);
	}
}

void wiggleFlareUpdate(Particle* particle, ParticleList& toAdd)
{
	particle->velocity += kGravity / 2;
	particle->position += particle->velocity;

	particle->position.x += (((particle->lifeTime >> 1) & 1) == 1) ? 1 : -1;

	if (particle->lifeTime < 5)
	{
		decrease(particle->color);
	}
}

void sparklerUpdate(Particle* particle, ParticleList& toAdd)
{
	particle->velocity += kGravity;
	particle->position += particle->velocity;

	if (particle->lifeTime < 5)
	{
		decrease(particle->color);
	}

	if (particle->lifeTime == 0)
	{
		if (randPercent(12) && sTexts.size() > 0)
		{
			const int messageIdx = randRange(0, (int)sTexts.size() - 1);
			addMessage(toAdd, particle->position, sTexts[messageIdx]);
		}
		else if (randPercent(12))
		{
			if (randPercent(15) || sImageFiles.size() <= 0)
			{
				addFlag(toAdd, particle->position);
			}
			else
			{
				const int imageIdx = randRange(0, (int)sImageFiles.size() - 1);

				addImage(toAdd, particle->position, *(sImageFiles[imageIdx]));
			}
		}
		else
		{
			addFlares(toAdd, particle->position);
		}
	}
}

void superSparkleUpdate(Particle* particle, ParticleList& toAdd)
{
	if ((particle->lifeTime & 0xF) == 0)
	{
		addSmallFlares(toAdd, particle->position, particle->color);
	}

	sparklerUpdate(particle, toAdd);
}

void imageUpdate(Particle* particle, ParticleList& toAdd)
{
	if (particle->varA-- > 0)
	{
		particle->position += particle->velocity;

		if (particle->varA == 0)
		{
			particle->velocity = Vector2<Fx32>(0, 0);
		}
	}
	else if (particle->lifeTime < 5)
	{
		decrease(particle->color);
	}
	else
	{
		particle->velocity += kGravity;
		particle->position += particle->velocity;
	}
}


//------------------------------------------------------------------------------
//	Fireworks
//------------------------------------------------------------------------------
void addSmallFlares(ParticleList& toAdd, const Vector2<Fx32>& position, const ARGB& color)
{
	const int numFlares = 4;
	const int lifeTime = randPercent(50) ? 10 : 20;
	const float angleAdd = float(M_PI * 2) / numFlares;
	const Fx32 velocityMul = Fx32(32 + randRange(-8, 16)) / 32;

	Particle partAdd;

	partAdd.position = position;
	partAdd.color = color;
	partAdd.lifeTime = lifeTime;

	if (randPercent(50))
	{
		partAdd.update = &flareUpdate;
	}
	else
	{
		partAdd.update = &wiggleFlareUpdate;
	}

	for (int i = 0; i < numFlares; ++i)
	{
		partAdd.velocity =
			Vector2<Fx32>(
				cosf(angleAdd * i + float(M_PI / 4)),
				sinf(angleAdd * i + float(M_PI / 4)))
			* velocityMul;

		toAdd.push_back(partAdd);
	}
}

void addFlares(ParticleList& toAdd, const Vector2<Fx32>& position)
{
	const int colorIdx = randRange(0, (int)ARRAY_LENGTH(kSparkleFlares) - 1);
	const ARGB flareColor = kSparkleFlares[colorIdx];

	const int numFlareIdx = randRange(0, (int)ARRAY_LENGTH(kSparkleFlareNum) - 1);
	const int numFlares = kSparkleFlareNum[numFlareIdx];
	const int lifeTime = randPercent(50) ? 20 : 40;
	const float angleAdd = float(M_PI * 2) / numFlares;
	const Fx32 velocityMul = Fx32(48 + randRange(-24, 32)) / 32;

	Particle partAdd;

	partAdd.position = position;
	partAdd.color = flareColor;
	partAdd.lifeTime = lifeTime;

	if (randPercent(50))
	{
		partAdd.update = &flareUpdate;
	}
	else if (randPercent(50))
	{
		partAdd.update = &superFlareUpdate;
		partAdd.varA = randRange(0, (int)ARRAY_LENGTH(kSparkleFlares) - 1);
	}
	else
	{
		partAdd.update = &wiggleFlareUpdate;
	}

	for (int i = 0; i < numFlares; ++i)
	{
		partAdd.velocity = Vector2<Fx32>(cosf(angleAdd * i), sinf(angleAdd * i)) * velocityMul;

		toAdd.push_back(partAdd);
	}

	if (randPercent(15))
	{
		addFlares(toAdd, position);
	}
}

void addMessage(ParticleList& toAdd, const Vector2<Fx32>& position, RefString text)
{
	if (sAlphabet.get() == NULL)
	{
		sAlphabet = SharedImage(Image::Key(IDB_BITMAP1, GetModuleHandle(NULL)));
	}

	const int colorIdx = randRange(0, (int)ARRAY_LENGTH(kSparkleFlares) - 1);
	const ARGB flareColor = kSparkleFlares[colorIdx];
	const int lifeTime = randPercent(50) ? 20 : 40;
	const size_t length = text.length();

	Particle partAdd;

	partAdd.position = position;
	partAdd.lifeTime = lifeTime;
	partAdd.varA = 8;
	partAdd.update = &imageUpdate;

	RGBQUAD palette[256];

	sAlphabet->palette(0, 256, palette);

	Vector2<Fx32> addPos = position - Vector2<Fx32>(Fx32((int)(length * 16) / 2), Fx32(8));

	for (size_t i = 0; i < length; ++i)
	{
		for (int y = 0; y < 8; ++y)
		{
			BYTE* pixels =
				sAlphabet->pixelPtr(
					(text[i] & 0xF) * 8,
					((text[i] >> 4) * 8) + y);

			for (int x = 0; x < 8; ++x)
			{
				const BYTE px = pixels[x];

				if (palette[px].rgbBlue == 0
					&& palette[px].rgbGreen == 0
					&& palette[px].rgbRed == 0)
				{
					continue;
				}

				//partAdd.position = addPos + Vector2<Fx32>(x * 2, y * 2);
				partAdd.velocity = ((addPos + Vector2<Fx32>(x * 2, y * 2)) - position) / 8;
				partAdd.color =
					ARGB(
						min(palette[px].rgbRed, flareColor.red),
						min(palette[px].rgbGreen, flareColor.green),
						min(palette[px].rgbBlue, flareColor.blue));
				
				toAdd.push_back(partAdd);
			}
		}

		addPos.x += 16;
	}
}

void addImage8(ParticleList& toAdd, const Vector2<Fx32>& position, Bitmap& image)
{
	const int colorIdx = randRange(0, (int)ARRAY_LENGTH(kSparkleFlares) - 1);
	const ARGB flareColor = kSparkleFlares[colorIdx];
	const int lifeTime = randPercent(50) ? 30 : 50;

	Particle partAdd;

	partAdd.lifeTime = lifeTime;
	//partAdd.update = &messageUpdate;
	partAdd.update = &imageUpdate;
	partAdd.position = position;

	RGBQUAD palette[256];

	image.palette(0, 256, palette);

	for (int y = 0; y < image.height(); ++y)
	{
		BYTE* pixels = image.linePtr(y);

		for (int x = 0; x < image.width(); ++x)
		{
			const BYTE px = pixels[x];

			if (palette[px].rgbBlue == 0
				&& palette[px].rgbGreen == 0
				&& palette[px].rgbRed == 0)
			{
				continue;
			}

			const Vector2<Fx32> desiredPos =
			//partAdd.position =
				position
				+ Vector2<Fx32>(x * 2, y * 2)
				- Vector2<Fx32>(image.width(), image.height());
			partAdd.varA = 8;
			partAdd.velocity = (desiredPos - position) / 8;

			partAdd.color =
				ARGB(
					palette[px].rgbRed,
					palette[px].rgbGreen,
					palette[px].rgbBlue);

			toAdd.push_back(partAdd);
		}
	}
}

void addImage16(ParticleList& toAdd, const Vector2<Fx32>& position, Bitmap& image)
{
	const int colorIdx = randRange(0, (int)ARRAY_LENGTH(kSparkleFlares) - 1);
	const ARGB flareColor = kSparkleFlares[colorIdx];
	const int lifeTime = randPercent(50) ? 30 : 50;

	Particle partAdd;

	partAdd.lifeTime = lifeTime;
	//partAdd.update = &messageUpdate;
	partAdd.update = &imageUpdate;
	partAdd.position = position;

	RGBQUAD palette[256];

	image.palette(0, 256, palette);

	for (int y = 0; y < image.height(); ++y)
	{
		WORD* pixels = (WORD*) image.linePtr(y);

		for (int x = 0; x < image.width(); ++x)
		{
			const RGBQUAD px = ARGB(pixels[x]);

			if (px.rgbBlue == 0
				&& px.rgbGreen == 0
				&& px.rgbRed == 0)
			{
				continue;
			}

			const Vector2<Fx32> desiredPos =
				//partAdd.position =
				position
				+ Vector2<Fx32>(x * 2, y * 2)
				- Vector2<Fx32>(image.width(), image.height());
			partAdd.varA = 8;
			partAdd.velocity = (desiredPos - position) / 8;
			partAdd.color = ARGB(px.rgbRed, px.rgbGreen, px.rgbBlue);

			toAdd.push_back(partAdd);
		}
	}
}

void addImage24(ParticleList& toAdd, const Vector2<Fx32>& position, Bitmap& image)
{
	const int colorIdx = randRange(0, (int)ARRAY_LENGTH(kSparkleFlares) - 1);
	const ARGB flareColor = kSparkleFlares[colorIdx];
	const int lifeTime = randPercent(50) ? 30 : 50;

	Particle partAdd;

	partAdd.lifeTime = lifeTime;
	//partAdd.update = &messageUpdate;
	partAdd.update = &imageUpdate;
	partAdd.position = position;

	RGBQUAD palette[256];

	image.palette(0, 256, palette);

	for (int y = 0; y < image.height(); ++y)
	{
		RGBTRIPLE* pixels = (RGBTRIPLE*)image.linePtr(y);

		for (int x = 0; x < image.width(); ++x)
		{
			const ARGB px(pixels[x].rgbtRed, pixels[x].rgbtGreen, pixels[x].rgbtBlue);

			if (px.red == 0
				&& px.green == 0
				&& px.blue == 0)
			{
				continue;
			}

			const Vector2<Fx32> desiredPos =
				//partAdd.position =
				position
				+ Vector2<Fx32>(x * 2, y * 2)
				- Vector2<Fx32>(image.width(), image.height());
			partAdd.varA = 8;
			partAdd.velocity = (desiredPos - position) / 8;
			partAdd.color = px;

			toAdd.push_back(partAdd);
		}
	}
}

void addImage32(ParticleList& toAdd, const Vector2<Fx32>& position, Bitmap& image)
{
	const int lifeTime = randPercent(50) ? 30 : 50;

	Particle partAdd;

	partAdd.lifeTime = lifeTime;
	//partAdd.update = &messageUpdate;
	partAdd.update = &imageUpdate;
	partAdd.position = position;

	for (int y = 0; y < image.height(); ++y)
	{
		RGBQUAD* pixels = (RGBQUAD*)image.linePtr(y);

		for (int x = 0; x < image.width(); ++x)
		{
			const RGBQUAD& px = pixels[x];

			if ((px.rgbBlue == 0
				&& px.rgbGreen == 0
				&& px.rgbRed == 0)
				|| px.rgbReserved == 0)
			{
				continue;
			}

			const Vector2<Fx32> desiredPos =
				//partAdd.position =
				position
				+ Vector2<Fx32>(x * 2, y * 2)
				- Vector2<Fx32>(image.width(), image.height());
			partAdd.varA = 8;
			partAdd.velocity = (desiredPos - position) / 8;
			partAdd.color = ARGB(px.rgbRed, px.rgbGreen, px.rgbBlue, px.rgbReserved);

			toAdd.push_back(partAdd);
		}
	}
}

void addImage(ParticleList& toAdd, const Vector2<Fx32>& position, Bitmap& image)
{
	switch (image.channels())
	{
	case 1:
		addImage8(toAdd, position, image);
		break;

	case 2:
		addImage8(toAdd, position, image);
		break;

	case 4:
		addImage32(toAdd, position, image);
		break;
	}
}

void addFlag(ParticleList& toAdd, const Vector2<Fx32>& position)
{
	if (sFlag.get() == NULL)
	{
		sFlag = SharedImage(Image::Key(IDB_BITMAP2, GetModuleHandle(NULL)));
	}

	addImage(toAdd, position, *sFlag);
}

void addSparkler(ParticleSystem& system, const Vector2i& dims)
{
	Particle partAdd;

	partAdd.position =
		Vector2<Fx32>(
			(dims.x / 2) + randRange(-dims.x / 8, dims.x / 8),
			dims.y);

	partAdd.velocity =
		Vector2<Fx32>(
			Fx32(randRange(-1024, 1024))/256,
			Fx32(-11) + (Fx32(randRange(-512, 512)/256)));

	const int colorIdx = randRange(0, (int)ARRAY_LENGTH(kSparkleFlares) - 1);
	partAdd.color = kSparkleFlares[colorIdx];

	if (randPercent(25))
	{
		partAdd.update = &superSparkleUpdate;
	}
	else
	{
		partAdd.update = &sparklerUpdate;
	}

	partAdd.lifeTime = randRange(70, 90);

	system.add(partAdd);
}

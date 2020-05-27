#include "iImage.h"

#include "iStd.h"

iImage::iImage()
{
	arrayTex = new iArray(freeTex);
	tex = NULL;
	position = iPointMake(0, 0);

	selected = false;
	selectedDt = 0.0f;
	_selectedDt = default_selectedDt;
	selectedScale = default_selectedScale;
	
	animation = false;
	aniDt = 0.0f;
	_aniDt = 0.08f;
	frame = 0;
	repeatNum = 0;
	_repeatNum = 0;// inf
	method = NULL;

	ratio = 1.0f;
	imgRatioX = 1.0f;
	imgRatioY = 1.0f;
	location = 2; // 0.x 1.y 2.z
	reverseRotate = false;
	angle = 0.0f;
	lockAngle = false;
}

iImage::~iImage()
{
	delete arrayTex;
}

void iImage::freeTex(void* data)
{
	Texture* tex = (Texture*)data;
	if (tex->retainCount > 1)
	{
		tex->retainCount--;
		return;
	}
	freeImage(tex);
}

void iImage::addObject(Texture* tex)
{
	arrayTex->addObject(tex);
	tex->retainCount++;
	if (this->tex == NULL)
		this->tex = tex;
}

void iImage::setTexAtIndex(int index)
{
	Texture* tex = (Texture*)arrayTex->objectAtIndex(index);
	if (tex)
	{
		this->tex = tex;
		frame = index;
	}
}

void iImage::replaceAtIndex(int index, Texture* tex)
{
	Texture* t = (Texture*)arrayTex->objectAtIndex(index);
#if 0
	arrayTex->remove(index);
	arrayTex->addObject(index, tex);
	if (this->tex == t)
		this->tex = tex;
#else
	//freeImage(t);
	if (t->retainCount > 1)
		t->retainCount--;
	else
		glDeleteTextures(1, &t->texID);
	memcpy(t, tex, sizeof(Texture));
	free(tex);
#endif
}

void iImage::paint(float dt, iPoint off,int reverse)
{
	if (animation)
	{
		aniDt += dt;
		if (aniDt < _aniDt)
		{
			;
		}
		else
		{
			aniDt -= _aniDt;
			frame++;
			if (frame == arrayTex->count)
			{
				if( _repeatNum==0 )
					frame = 0;
				else
				{
					repeatNum++;
					if (repeatNum < _repeatNum)
						frame = 0;
					else// if (repeatNum == _repeatNum)
					{
						if (method)
							method(this);
						if (lastFrame)
							frame = arrayTex->count - 1;
						else
							frame = 0;
						animation = false;
					}
				}
			}
			tex = (Texture*)arrayTex->objectAtIndex(frame);
		}
	}

	if (selected && selectedDt < _selectedDt)
	{
		selectedDt += dt;
		if (selectedDt > _selectedDt)
			selectedDt = _selectedDt;
	}
	else
	{
		selected = false;
		selectedDt = 0.0f;
	}

	float a = 0.0f; 
	if (reverseRotate) a = 360 - linear(selectedDt / _selectedDt, 0.0f, angle);
	else a = linear(selectedDt / _selectedDt, 0.0f, angle);

	iPoint p = position + off;
	float s = 1.0f - linear(selectedDt / _selectedDt, 0.0f, selectedScale);

	if (s == 0.0f)
	{
		drawImage(tex, p.x, p.y, VCENTER | HCENTER);
	}
	else if (lockAngle)
	{
		p.x += tex->width / 2 * ratio * imgRatioX;
		p.y += tex->height / 2 * ratio * imgRatioY;
		drawImage(tex, p.x , p.y, 0, 0, tex->width * imgRatioX, tex->height * imgRatioY,
			VCENTER | HCENTER, s * ratio * imgRatioX, s * ratio * imgRatioY, location, angle, reverse);
	}
	else
	{
		p.x += tex->width / 2 * ratio * imgRatioX;
		p.y += tex->height / 2 * ratio * imgRatioY;
		drawImage(tex, p.x, p.y, 0, 0, tex->width * imgRatioX, tex->height * imgRatioY,
			VCENTER | HCENTER, s * ratio * imgRatioX, s * ratio * imgRatioY, location, a, reverse);
	}
}

void iImage::startAnimation(IMAGE_METHOD m)
{
	animation = true;
	method = m;
	aniDt = 0.0f;
	frame = 0;
	repeatNum = 0;
}

iRect iImage::touchRect(iPoint p)
{
	p += position;
	iRect rt = iRectMake(p.x, p.y,
						tex->width, tex->height);
	return rt;
}

iImage* iImage::copy()
{
#if 0
	iArray* array = new iArray(freeTex);
	for (int i = 0; i < arrayTex->count; i++)
	{
		//array->addObject(arrayTex->objectAtIndex(i));
		Texture* tex = (Texture*)arrayTex->objectAtIndex(i);
		array->addObject(tex);
		tex->retainCount++;
	}

	iImage* img = new iImage();
	memcpy(img, this, sizeof(iImage));
	img->arrayTex = array;
#else
	iImage* img = new iImage();
	memcpy(img, this, sizeof(iImage));
	img->arrayTex = new iArray(freeTex);
	for (int i = 0; i < arrayTex->count; i++)
		img->addObject((Texture*)arrayTex->objectAtIndex(i));
#endif

	return img;
}


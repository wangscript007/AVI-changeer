#include "aviCahngger.h"


aviChanger::aviChanger(std::string sourceName, float speed) :
	m_sourceName(sourceName), m_speed(speed)
{
	if (speed<0.00001f)
	{
		std::cout << "speed " << speed << " too small" << std::endl;
		return;
	}
	doChange();
}

aviChanger::~aviChanger()
{
	if (m_fpSource)
	{
		fclose(m_fpSource);
		m_fpSource = nullptr;
	}
}

bool aviChanger::doChange()
{
	bool result = true;
	do
	{

		m_fpSource = fopen(m_sourceName.c_str(), "rb+");
		if (!m_fpSource)
		{
			std::cout << "open file failed:" << m_sourceName << std::endl;
			result = false;
			break;
		}
		changeVideoStrhRate();
	} while (0);

	return result;
}

void aviChanger::changeVideoStrhRate()
{
	do
	{
		char tmpFourCC[4];
		const int fourbit = 4;
		size_t readCount;
		unsigned long dwSize;
		//RIFF
		readCount=fread(tmpFourCC, fourbit, 1, m_fpSource);
		if (readCount!=1)
		{
			std::cout << "read file failed;" << std::endl;
			break;
		}
		if (strncmp(tmpFourCC,AVI_RIFF,fourbit)!=0)
		{
			std::cout << "invalid avi";
		}
		readCount = fread(&dwSize, fourbit, 1, m_fpSource);
		if (readCount != 1)
		{
			std::cout << "read file failed;" << std::endl;
			break;
		}
		//AVI
		readCount = fread(tmpFourCC, fourbit, 1, m_fpSource);
		if (readCount != 1)
		{
			std::cout << "read file failed;" << std::endl;
			break;
		}
		if (strncmp(tmpFourCC, AVI_AVI, fourbit) != 0)
		{
			std::cout << "invalid avi";
		}
		//LIST
		readCount = fread(tmpFourCC, fourbit, 1, m_fpSource);
		if (readCount != 1)
		{
			std::cout << "read file failed;" << std::endl;
			break;
		}
		if (strncmp(tmpFourCC, AVI_LIST, fourbit) != 0)
		{
			std::cout << "invalid avi";
		}
		readCount = fread(&dwSize, fourbit, 1, m_fpSource);
		if (readCount != 1)
		{
			std::cout << "read file failed;" << std::endl;
			break;
		}
		parseList(dwSize);
	} while (0);
}

void aviChanger::parseList(unsigned long size)
{
	char tmpFourCC[4];
	const int fourbit = 4;
	size_t readCount;
	unsigned long dwSize;
	unsigned long cur = 0;
	while (cur<size)
	{
		readCount = fread(tmpFourCC, fourbit, 1, m_fpSource);
		cur += fourbit;
		if (readCount != 1)
		{
			std::cout << "read file failed;" << std::endl;
			break;
		}
		if (strncmp(tmpFourCC,AVI_LIST,fourbit)==0)
		{
			readCount = fread(&dwSize, fourbit, 1, m_fpSource);
			cur += 4;
			if (readCount != 1)
			{
				std::cout << "read file failed;" << std::endl;
				break;
			}
			parseList(dwSize);
			cur += dwSize;
		}
		else if (strncmp(tmpFourCC, AVI_HDRL, fourbit) == 0)
		{
			continue;
		}
		else if (strncmp(tmpFourCC, AVI_AVIH, fourbit) == 0)
		{
			readCount = fread(&dwSize, fourbit, 1, m_fpSource);
			cur += 4;
			if (readCount != 1)
			{
				std::cout << "read file failed;" << std::endl;
				break;
			}
			fseek(m_fpSource, dwSize, SEEK_CUR);
			cur += dwSize;
		}
		else if (strncmp(tmpFourCC, AVI_STRL, fourbit) == 0)
		{
			continue;
		}
		else if (strncmp(tmpFourCC, AVI_STRH, fourbit) == 0)
		{
			readCount = fread(&dwSize, fourbit, 1, m_fpSource);
			cur += 4;
			if (readCount != 1)
			{
				std::cout << "read file failed;" << std::endl;
				break;
			}
			//process strh
			m_videoStreamHeaderPos = ftell(m_fpSource);
			m_videoStreamHeaderSize = dwSize;

			readCount = fread(&m_videoStreamHeader, dwSize, 1, m_fpSource);
			if (readCount != 1)
			{
				std::cout << "read file failed;" << std::endl;
				break;
			}
			//if (strncmp((const char*)&m_videoStreamHeader.fccType,AVI_VIDS,fourbit)==0)
			if(true)
			{
				fseek(m_fpSource, -dwSize, SEEK_CUR);
				long nowPos = ftell(m_fpSource);
				m_videoStreamHeader.dwRate *= m_speed;
				if (m_videoStreamHeader.dwRate<=0)
				{
					m_videoStreamHeader.dwRate = 1;
				}
				fwrite(&m_videoStreamHeader, m_videoStreamHeaderSize, 1, m_fpSource);
				fflush(m_fpSource);
				nowPos = ftell(m_fpSource);
				fseek(m_fpSource, nowPos, SEEK_SET);

				/*fflush(m_fpSource);
				fclose(m_fpSource);
				m_fpSource = 0;
				m_fpSource = fopen(m_sourceName.c_str(), "rb+");
				fseek(m_fpSource, nowPos, SEEK_CUR);*/
				//exit(0);
				nowPos = 0;
			}
			else
			{
				printf("not vids\n");
			}
			//!process strh
			//break;
			cur += dwSize;
		}
		else if (strncmp(tmpFourCC, AVI_STRF, fourbit) == 0)
		{
			readCount = fread(&dwSize, fourbit, 1, m_fpSource);
			cur += 4;
			if (readCount != 1)
			{
				std::cout << "read file failed;" << std::endl;
				break;
			}
			fseek(m_fpSource, dwSize, SEEK_CUR);
			cur += dwSize;
		}
		else if (strncmp(tmpFourCC, AVI_JUNK, fourbit) == 0)
		{
			readCount = fread(&dwSize, fourbit, 1, m_fpSource);
			cur += 4;
			if (readCount != 1)
			{
				std::cout << "read file failed;" << std::endl;
				break;
			}
			fseek(m_fpSource, dwSize, SEEK_CUR);
			cur += dwSize;
		}
		else
		{
			break;
		}
	}
}

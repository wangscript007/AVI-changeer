#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <Vfw.h>
#include <string>
#include <iostream>



#define AVI_RIFF "RIFF"
#define AVI_AVI  "AVI "
#define AVI_LIST "LIST"
#define AVI_HDRL "hdrl"
#define AVI_AVIH "avih"
#define AVI_STRL "strl"
#define AVI_STRH "strh"
#define AVI_STRF "strf"
#define AVI_JUNK "JUNK"
#define AVI_MOVI "movi"
#define AVI_REC  "rec"
#define AVI_INDX "idx1"
#define AVI_VIDS "vids"


class aviChanger
{
public:
	aviChanger(std::string sourceName,float speed);
	~aviChanger();
private:
	bool doChange();
	void changeVideoStrhRate();
	void parseList(unsigned long size);
	FILE *m_fpSource;
	std::string m_sourceName;
	float m_speed;
	long m_videoStreamHeaderPos;
	AVIStreamHeader	m_videoStreamHeader;
	unsigned int m_videoStreamHeaderSize;
};


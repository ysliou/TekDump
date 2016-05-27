#include <StdAfx.h>

/** \mainpage	Tektronix TeleScope Screen Dumper 

\author	    �B�y��
\version    V1.0
\date	    2006/03/26

\par
�����ԭz Tektronix �ܪi���ù��^���n�骺�]�p�Ӹ`�C

\section   software	�n��[�c

\dot
digraph system
{
	node [shape=box, height=0.1, fontname=Helvetica, fontsize=9];
	labelfontname=Helvetica;
	labelloc="b";
	color=gray;
	ranksep=0.6;

	app	[label="TekDump",			URL="\ref CTekDumpApp"];
	doc	[label="TekDumpDoc",		URL="\ref CTekDumpDoc"];
	frame	[label="CMainFrame",	URL="\ref CMainFrame"];	
	view	[label="TekDumpView",	URL="\ref CTekDumpView"];
	setup	[label="Setup",			URL="\ref CSetup"];
	imageio	[label="ImageIO",		URL="\ref ImageIO.cpp"];

	app->doc->frame->view;
	app->setup;
	app->imageio;
}
\enddot

\note
�����ϥ� Doxygen �n�鲣�͡A
��󪺤��e�O�H�S�w���y�k�H���Ѫ��覡�������O���l�X���A
�]���i�H�������ԭz�P�{���X�����פ@�P�ʡC    

*/

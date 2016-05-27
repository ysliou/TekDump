#include <StdAfx.h>

/** \mainpage	Tektronix TeleScope Screen Dumper 

\author	    劉宜生
\version    V1.0
\date	    2006/03/26

\par
本文件敘述 Tektronix 示波器螢幕擷取軟體的設計細節。

\section   software	軟體架構

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
本文件使用 Doxygen 軟體產生，
文件的內容是以特定的語法以註解的方式直接註記於原始碼中，
因此可以維持文件敘述與程式碼的高度一致性。    

*/

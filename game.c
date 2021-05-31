#include<SDL.h>

#include<stdio.h>

#include<stdlib.h>

#include<SDL_ttf.h>



int main()

{
	//定义部分
	SDL_Surface *screen,*text;

	//图像指针，image2代表人物，image代表怪物

	SDL_Surface *image2,*image;

	SDL_Event event; //事件对象

	TTF_Font *Nfont;  //文字样式对象

	Uint32 color;

	//程序运行和人物运动方向标志

	int bAppRun=1,right,left,up,down;

	int bTopBottom=1;  //屏幕上下

	int bLeftRight=1;  //屏幕左右

	Uint32 Tstart,Tstop;  //时间开始和结束

	//目标矩形，分别代表怪物、人物、文字

	SDL_Rect dRect,dRect2,dRect3;

	

	

	//初始化视频子系统和计时器子系统

	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)<0)

	{

		fprintf(stderr,"不能初始化:%s",SDL_GetError());

		exit(1);

	}

	if((screen=SDL_SetVideoMode(960,720,16,SDL_SWSURFACE))==NULL)

	{

	fprintf(stderr,"不能初始化960*720,16位色的视频模式：%s\n",SDL_GetError());

		exit(1);

	}

	atexit(SDL_Quit);

	//人物和怪物初始化	
	image2=SDL_LoadBMP("2.bmp");  //加载人物图片

	image=SDL_LoadBMP("1.bmp");  //加载怪物图片

	if(image==NULL||image2==NULL){

		fprintf(stderr,"不能加载BMP，%s\n",SDL_GetError());

		exit(1);

	}

	//怪物左上角的坐标

	dRect.x=0;

	dRect.y=0;

	//宽和高为位图的宽和高

	dRect.w=image->w;

	dRect.h=image->h;

	if(SDL_BlitSurface(image,NULL,screen,&dRect)<0){

		fprintf(stderr,"BlitSurface error: %s\n",SDL_GetError());

		SDL_FreeSurface(image);

		exit(1);

	}

	//人物左上角的坐标

	dRect2.x=0;

	dRect2.y=480;

	dRect2.w=image2->w;

	dRect2.h=image2->h;

	if(SDL_BlitSurface(image2,NULL,screen,&dRect2)<0){

		fprintf(stderr,"BlitSurface error: %s\n",SDL_GetError());

		SDL_FreeSurface(image);

		exit(1);

	}

	

	SDL_UpdateRect(screen,0,0,0,0);

	

	Tstart=SDL_GetTicks();  //计时开始

	while(bAppRun==1){

		while(SDL_PollEvent(&event)){

			if(event.type==SDL_KEYDOWN)  

				switch(event.key.keysym.sym){  //判断按键类型

					//按下ESC直接退出

					case SDLK_ESCAPE:

					bAppRun=0;

					break;

					//按下右键，有运动标志为真，其余为假

					case SDLK_RIGHT:

					right=1;

					left=0;

					up=0;

					down=0;

					break;

					//按下左键

					case SDLK_LEFT:

					left=1;

					right=0;

					up=0;

					down=0;

					break;

					//按下上键

					case SDLK_UP:

					up=1;

					right=0;

					left=0;

					down=0;

					break;

					//按下下键

					case SDLK_DOWN:

					down=1;

					right=0;

					left=0;

					up=0;

					break;

				}

		}
		
		Tstop=SDL_GetTicks();  //计时
		//怪物的移动
		if((Tstop-Tstart)>5){
			//将当前怪物图像区域填充为黑色
			SDL_FillRect(screen,&dRect,0);
			//如果碰到屏幕上部或下部
			if(bTopBottom==1){  
				//如果没碰到下部，则向下移动(目前在上部)
				if((dRect.y+dRect.h+3)<screen->h){

					dRect.y+=3;

				}
				//若将碰到下部，则向上移动（目前在下部）
				else{

					bTopBottom=0;

					dRect.y=screen->h-dRect.h;

				}

			}
			//如果在向上移动中
			else{
				//没有碰到上部，则向上移动
				if((dRect.y-3)>0){

					dRect.y-=3;

				}
				//将碰到上部，则设置碰到上部标志为1
				else{

					bTopBottom=1;

					dRect.y=0;

				}

			}
			//如果碰到屏幕左部或右部
			if(bLeftRight==1){
				//如果没有碰到右部，向右移动
				if((dRect.x+dRect.w+3)<screen->w){

					dRect.x+=3;

				}
				//如果碰到右部，向左移动
				else{

					bLeftRight=0;

					dRect.x=screen->w-dRect.w;

				}

			}
			//向左移动中
			else{
				//如果没有碰到左部，向左移动
				if((dRect.x-3)>0){

					dRect.x-=3;

				}
				//如果碰到左部，设置碰到右标志为1
				else{

					bLeftRight=1;

					dRect.x=0;

				}

			}

			//把目标快速转化

			if(SDL_BlitSurface(image,NULL,screen,&dRect)<0){

				fprintf(stderr,"BlitSurface error: %s\n",SDL_GetError());

				SDL_FreeSurface(image);

			}

			SDL_Flip(screen);  //屏幕缓冲

		}
		//人物的移动
		if((Tstop-Tstart)>15){
			//人物的移动要比怪物慢
			Tstart=Tstop;  //设置新的开始时间
			//将当前人物图像区域填充为黑色
			SDL_FillRect(screen,&dRect2,0);

			if(right==1)

			{

				//判断是否碰到屏幕右边缘,不是则向右移动

				if((dRect2.x+dRect2.w+3)<screen->w)

					dRect2.x+=3;

			}

			if(left==1)

			{

				//判断是否碰到屏幕左边缘,不是则向左移动

				if((dRect2.x-3)>0)

					dRect2.x-=3;

			}

			if(up==1)

			{

				//判断是否碰到屏幕上边缘,不是则向上移动

				if((dRect2.y-3)>0)

					dRect2.y-=3;

			}

			if(down==1)

			{

				//判断是否碰到屏幕下边缘,不是则向下移动

				if((dRect2.y+dRect2.h+3)<screen->h)

					dRect2.y+=3;

			}

			//把目标快速转化

			if(SDL_BlitSurface(image2,NULL,screen,&dRect2)<0){

				fprintf(stderr,"BlitSurface error: %s\n",SDL_GetError());

				SDL_FreeSurface(image2);

			}

			SDL_Flip(screen);  //屏幕缓冲

		}

		//判断人物与怪物是否接触
		//人物左右边界碰到或在怪物的左右边界内
		if((dRect2.x>=dRect.x-image2->w)&&(dRect2.x<=dRect.x+image->w))
			//人物上下边界碰到或在怪物上下边界内
			if((dRect2.y>=dRect.y-image2->h)&&(dRect2.y<=dRect.y+image->h))

				break;

	}

	SDL_FreeSurface(image);

	SDL_FreeSurface(image2); /*释放图像*/

	int x;
	//使背景逐渐由红变白
	for(x=0;x<=255;x++){ /*用循环来实现背景颜色*/

		color=SDL_MapRGB(screen->format,255,x,x); /*设置颜色*/

		SDL_FillRect(screen,NULL,color); /*填充整个屏幕*/

		SDL_UpdateRect(screen,0,0,0,0); /*更新整个屏幕*/

		SDL_Delay(5);

	}

	//显示gameover

	SDL_Color red= {255,0,0}; /*设置字体颜色*/

	int fontsize=70; /*设置字体大小*/

	if(TTF_Init()!=0){ /*初始化字体*/

		fprintf(stderr,"Can't init ttf font!\n");

		exit(1);

	}

	Nfont=TTF_OpenFont("simsun.ttc",fontsize);

	//Nfont=TTF_OpenFont("/usr/share/fonts/truetype/droid/DroidSansFallbackFull.ttf",fontsize); /*打开字体库*/

	TTF_SetFontStyle(Nfont,TTF_STYLE_NORMAL); /*设置字体样式*/

	text=TTF_RenderUTF8_Blended(Nfont,"GAME OVER",red);

	TTF_CloseFont(Nfont); /*关闭字体库*/

	TTF_Quit(); /*退出*/

	dRect3.x=200; /*在点（200，300处开始写）*/

	dRect3.y=300;

	dRect3.w=text->w; /*目标矩形的宽和高分比的是所写字的宽和高*/

	dRect3.h=text->h;

	SDL_BlitSurface(text,NULL,screen,&dRect3); /*把目标快速转化*/

	SDL_UpdateRect(screen,0,0,0,0);

	SDL_FreeSurface(text);

	SDL_Delay(3000); /*停留3秒*/

	return 0;

}


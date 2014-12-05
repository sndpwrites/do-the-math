//header files
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include "string"
#include "iostream"
#include "stdlib.h"
#include "time.h"
#include "fstream"
#include "sstream"
using namespace std;
ifstream inf("questionset.txt");

void motionevents(int);
const int SCREEN_WIDTH = 1366;
const int SCREEN_HEIGHT = 768;
const int SCREEN_BPP = 32;
const int button_width = 75;
const int button_height = 75;
bool valueentered=false;
bool quitcheck = false;
bool clickstart = false;
bool mainmenu = false;
int num=0, symbol,noofquestions=0,score=0;
string uservalue,ans;
//Global Declaration
SDL_Surface* blocking  = NULL;
SDL_Surface* screen = NULL;
SDL_Surface* background = NULL;
SDL_Surface* title = NULL;
SDL_Surface* startbutton = NULL;
SDL_Surface* startbutton_hover = NULL;
SDL_Surface* pausebutton = NULL;
SDL_Surface* puzzle_image[10]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
SDL_Surface* play_hover = NULL;
SDL_Surface* howtoplay_hover = NULL;
SDL_Surface* highscore_hover = NULL;
SDL_Surface* quit_hover = NULL;
SDL_Surface* sign_add = NULL;
SDL_Surface* sign_subtract = NULL;
SDL_Surface* sign_equals = NULL;
SDL_Surface* sign_multiply = NULL;
SDL_Surface* sign_divide = NULL;
TTF_Font *font_numbers = NULL;
TTF_Font *font_others = NULL;
TTF_Font *font_howtoplay = NULL;
SDL_Color wait_title_text = {255,255,255}, random_color={159,206,57};
SDL_Color correct_text = {0,0,0}, correct_bg={0,255,0};
SDL_Color incorrect_text = {0,0,0}, incorrect_bg={255,0,0};
SDL_Color score_color = {132,148,197};
SDL_Color textColor = { 22, 233, 75};
SDL_Color white={255,255,255};
SDL_Color red={255,0,0};
SDL_Color black={19,7,245};

SDL_Surface* begin = NULL;
SDL_Surface* correct = NULL;
SDL_Surface *gameover=NULL;
SDL_Surface *input_disp=NULL;
SDL_Surface *score_disp=NULL;
SDL_Surface *ename=NULL;
SDL_Surface *timetaken=NULL;

//for music
Mix_Music *music = NULL;
Mix_Chunk *high = NULL;

//for event handling
SDL_Event event;
SDL_Surface *load_image( std::string filename );
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination );
void play_window();
void clean_up();
void playbutton();
bool init();
bool load_files();
void load_buttons();
void load_buttons2();
void checkcorrect(string);
void setsign();
void setans();
void startgame();
void screenrefresh();
void handle_events();
void levelone(int);
int audio();
void gameoverdisplay(int);
void highscoremenu();
void howtoplaymenu();
template<class T1>
  T1 adata(T1 m,T1 y)	//to check score
    {
    return(m>y?m:y);
    }
template<class T2>
bool dataequals(T2 a,T2 b)	//to check answer
    {
    return(a==b?true:false);
    }


void motionevents(int)	//for motion events
{
int x,y;	
SDL_Rect offset;
int a=0;
	x=event.motion.x;
	y=event.motion.y;
	if ((y>(SCREEN_HEIGHT-590))&&(y<(SCREEN_HEIGHT-590+56)))
	a=590;	//play
	if ((y>(SCREEN_HEIGHT-520))&&(y<(SCREEN_HEIGHT-520+56)))
	a=520;	//highscore
	if ((y>(SCREEN_HEIGHT-450))&&(y<(SCREEN_HEIGHT-450+56)))
	a=450;	//howtoplay
	if ((y>(SCREEN_HEIGHT-380))&&(y<(SCREEN_HEIGHT-380+56)))
	a=380;	//quit
	offset.x = (SCREEN_WIDTH-200);		
	offset.y = (SCREEN_HEIGHT-a);
//the hover effect
	if ( ( x > (SCREEN_WIDTH-200) ) && ( x < (SCREEN_WIDTH))&&( y > (SCREEN_HEIGHT-a) ) && ( y < (SCREEN_HEIGHT-a) + 56 ) )
        {
			switch (a) {
			case 590:	//hover play
		            SDL_BlitSurface(play_hover, NULL, screen, &offset);
			break;
			case 520:
		            SDL_BlitSurface(highscore_hover, NULL, screen, &offset);
			break;
			case 450:
		            SDL_BlitSurface(howtoplay_hover, NULL, screen, &offset);
			break;
			case 380:
		            SDL_BlitSurface(quit_hover, NULL, screen, &offset);
			break;
			default:
			;
			}
	}
	else	//outside the buttons
	{
		apply_surface(0,0, background, screen);
			
	}
SDL_Flip(screen);
}

void howtoplaymenu()
{
SDL_Surface* temp = NULL;

temp = TTF_RenderText_Solid(font_howtoplay,"The rules to play Do-The-Math are very simple. Just answer the questions shown on your screen!",red);
apply_surface(0,100,temp,screen);
temp = TTF_RenderText_Solid(font_howtoplay,">Use the keyboard to enter your answer, and press Return key to check.",textColor);
apply_surface(0,200,temp,screen);
temp = TTF_RenderText_Solid(font_howtoplay,">Use Backspace if you made a mistake. Remember!! Once you press enter, there is no going back!",textColor);
apply_surface(0,300,temp,screen);
temp = TTF_RenderText_Solid(font_howtoplay,">Each question is worth 10 points. Answer all questions to your name on the high score board!",textColor);
apply_surface(0,400,temp,screen);
temp = TTF_RenderText_Solid(font_howtoplay,">Return to main menu anytime by pressing the Escape key.",textColor);
apply_surface(0,500,temp,screen);
SDL_Flip(screen);
/*getting keypress event*/ Uint8* keystates = SDL_GetKeyState( NULL );
        while( SDL_PollEvent( &event ) )
 	{
//Pressing escape will load the main menu
if (keystates[SDLK_ESCAPE])
{
	mainmenu=false;
	clickstart=false;
	background = load_image("images/background.png");
	load_buttons();

}
	}

}


string inttostr(int x)//function to convert integer to string
      {
stringstream ss;
ss<<x;
string str=ss.str();
return str;
}

void setsign(int x)
{
	switch (x)
	{
	case 1:
    	apply_surface((SCREEN_WIDTH-625),250, sign_add,screen);
	break;
	case 2:
	apply_surface((SCREEN_WIDTH-625),250, sign_subtract,screen);
	break;
	case 3:
	apply_surface((SCREEN_WIDTH-625),250, sign_multiply,screen);
	break;
	case 4:
	apply_surface((SCREEN_WIDTH-625),250, sign_divide,screen);
	default:
	;
	}
SDL_Flip(screen);
}	

void levelone(int x)
{
	mainmenu=true;	//menu no longer displayed
	SDL_Surface* temp = NULL; 
	int first,second,sign;
	temp = TTF_RenderText_Solid(font_numbers,inttostr(score).c_str(),score_color);
	apply_surface(800,15, temp, screen);	
	inf>>first;
	inf>>second;
	inf>>ans;
	inf>>sign;
	setsign(sign);    	
	apply_surface((SCREEN_WIDTH-375),250, sign_equals, screen);
	temp = TTF_RenderText_Solid(font_numbers,inttostr(first).c_str(),wait_title_text);
	apply_surface((SCREEN_WIDTH-750), 250, temp, screen);
	temp = TTF_RenderText_Solid(font_numbers,inttostr(second).c_str(),wait_title_text);
	apply_surface((SCREEN_WIDTH-750+250), 250, temp, screen);
	SDL_Flip(screen);

}
void handle_events()	//event handler
{
if (mainmenu==false)	//if menu is still displayed
{
if (clickstart==true)
	levelone(0);
else
	playbutton();	//clicking on a button

}
}

void screenrefresh(int x)
{
	load_buttons();
	load_buttons2();
	levelone(x);
	valueentered=false;
	SDL_Flip(screen);
}

void playbutton()	//if clicked on any button
{

int x,y,a;
SDL_Rect offset;
x=event.button.x;
y=event.button.y;
if ((y>(SCREEN_HEIGHT-590))&&(y<(SCREEN_HEIGHT-590+56)))
	a=590;	//play
	if ((y>(SCREEN_HEIGHT-520))&&(y<(SCREEN_HEIGHT-520+56)))
	a=520;	//highscore
	if ((y>(SCREEN_HEIGHT-450))&&(y<(SCREEN_HEIGHT-450+56)))
	a=450;	//howtoplay
	if ((y>(SCREEN_HEIGHT-380))&&(y<(SCREEN_HEIGHT-380+56)))
	a=380;	//quit
	offset.x = (SCREEN_WIDTH-200);		
	offset.y = (SCREEN_HEIGHT-a);
	if( ( x > (SCREEN_WIDTH-200) ) && ( x < SCREEN_WIDTH ) && ( y > (SCREEN_HEIGHT-a) ) && ( y < (SCREEN_HEIGHT-a) + 56 ))
	{
		switch (a) {
		case 590:	//play		
		clickstart = true;
		background=load_image("images/background.jpg");
		apply_surface(0,0,background,screen);
		load_buttons2();		
		break;

		case 520:	//highscore
		mainmenu=true;		
		background = load_image("images/background.jpg");
		load_buttons();
		highscoremenu();
		break;

		case 450:	//howtoplay
		mainmenu=true;
		background = load_image("images/background.jpg");
		load_buttons();
		howtoplaymenu();
		break;

		case 380:	//quit
			apply_surface(0,0,background,screen);
			quitcheck = true;
			return;
			break;
		default:
		;
		}
	}
SDL_Flip(screen);
handle_events();
}

void checkcorrect(string x)
{
SDL_Surface* temp = NULL;
	if (dataequals(x,ans)==true)	{
		correct = TTF_RenderText_Shaded(font_others,"CORRECT!! :)",correct_text,correct_bg);
		score+=10;
		apply_surface(50,(SCREEN_HEIGHT/5),puzzle_image[noofquestions],screen);
	}
	else
	{
		correct = TTF_RenderText_Shaded(font_others,"INCORRECT :(",incorrect_text,incorrect_bg);
	}

	
	apply_surface((SCREEN_WIDTH/2),(SCREEN_HEIGHT/2),correct,screen);
	SDL_Flip(screen);
	SDL_Delay(500);
	noofquestions++;
	num=0;
	if (noofquestions<50)
	{
	
	screenrefresh(0);
	
	}
	if (noofquestions==50)
	{
		
		SDL_Delay(500);
		temp = TTF_RenderText_Solid(font_others,inttostr(noofquestions).c_str(),wait_title_text);
		apply_surface((SCREEN_WIDTH/3),(SCREEN_HEIGHT/1.5),temp,screen);
		correct = TTF_RenderText_Solid(font_others," questions completed. Please Wait",wait_title_text);
		apply_surface((SCREEN_WIDTH/3)+20,(SCREEN_HEIGHT/1.5),correct,screen);
		SDL_Flip(screen);
		SDL_Delay(2000);
		gameoverdisplay(score);
	}
}
   class HighScoreDisplay
{
    private:
    string str;
    //The text surface
    SDL_Surface *text;
    SDL_Surface *te;
    public:
    //Initializes variables
    HighScoreDisplay();//Does clean up
    ~HighScoreDisplay();
    string file(string temp);
    void display_high();
    string sadan(string NUM);
};
    HighScoreDisplay::HighScoreDisplay()
{
    //Initialize the string
    str = "";

    //Initialize the surface
    text = NULL;

    //Enable Unicode
    SDL_EnableUNICODE( SDL_ENABLE );
}

HighScoreDisplay::~HighScoreDisplay()
{
    //Free text surface
    SDL_FreeSurface( text );

    //Disable Unicode
    SDL_EnableUNICODE( SDL_DISABLE );
}
string HighScoreDisplay::sadan(string NUM)
{

ifstream myfile("inte.txt");
myfile>>NUM;
return NUM;

}
string HighScoreDisplay::file(string temp)
{
ifstream fin("record.txt");
fin>>temp;
      return temp;
}

void HighScoreDisplay::display_high()
{
    string temp,NUM;
    text=TTF_RenderText_Solid( font_numbers, file(temp).c_str(), black );
    apply_surface((SCREEN_WIDTH/3)+250, (SCREEN_HEIGHT-400),text, screen);
    te=TTF_RenderText_Solid(font_numbers,sadan(NUM).c_str(),black);
    apply_surface((SCREEN_WIDTH/3)+250, (SCREEN_HEIGHT-400+100),te, screen);
    SDL_Flip(screen);
    }

void highscoremenu()
{
HighScoreDisplay name;
SDL_Surface* message = NULL;
    //Render the text
    message = TTF_RenderText_Solid( font_numbers, "Current Highscore", textColor );
    ename=TTF_RenderText_Solid( font_numbers, "name", textColor );
    score_disp=TTF_RenderText_Solid( font_numbers, "score", textColor );
    int i=400;
	load_buttons();
    apply_surface(( SCREEN_WIDTH - message->w ) / 2, ( SCREEN_HEIGHT - message->h ) / 3, message, screen);

    apply_surface((SCREEN_WIDTH/3.25), (SCREEN_HEIGHT-i),ename, screen);
    apply_surface((SCREEN_WIDTH/3.25), (SCREEN_HEIGHT-i+100),score_disp, screen);



    //Update the screen
    if( SDL_Flip( screen ) == -1 )
    {
        return;
    }
    name.display_high();
return;
/*getting keypress event*/ Uint8* keystates = SDL_GetKeyState( NULL );
        while( SDL_PollEvent( &event ) )
 	{
//Pressing escape will load the main menu
if (keystates[SDLK_ESCAPE])
{
	mainmenu=false;
	clickstart=false;
	background = load_image("images/background.png");
	load_buttons();

}
	}
}

class StringInput
{
    private:
    //The storage string
    std::string str;

    //The text surface
    SDL_Surface *text;
	TTF_Font *font;

    public:
    //Initializes variables
    StringInput();

    //Does clean up
    ~StringInput();
void recnum(int j);
    //Handles input
    void handle_input();

    //Shows the input on screen
    void show_centered();
    void record();
    int sadan();

};
StringInput::StringInput()
{
    //Initialize the string
    str = "";

    //Initialize the surface
    text = NULL;
    font = TTF_OpenFont( "font.ttf", 70);
    //Enable Unicode
    SDL_EnableUNICODE( SDL_ENABLE );
}
void StringInput::recnum(int j)
{
    int i=j;
  ofstream inte;
    inte.open("inte.txt");
    inte<<i;
    inte.close();
}
StringInput::~StringInput()
{
    //Free text surface
    SDL_FreeSurface( text );

    //Disable Unicode
    SDL_EnableUNICODE( SDL_DISABLE );
}
int StringInput::sadan()
{
   string result;
    int num;
    ifstream myfile("inte.txt");
        while(!myfile.eof())
            {
            getline(myfile,result);
            stringstream(result)>>num;
            }
            return num;

}
void StringInput::handle_input()
{
    //If a key was pressed
    if( event.type == SDL_KEYDOWN )
    {
        //Keep a copy of the current version of the string
string temp = str;



        //If the string less than maximum size
        if( str.length() <= 16 )
        {
                   //If the key is a space
            if( event.key.keysym.unicode == (Uint16)' ' )
            {
                //Append the character
                str += (char)event.key.keysym.unicode;
            }
                //If the key is a number
            else if( ( event.key.keysym.unicode >= (Uint16)'0' ) && ( event.key.keysym.unicode <= (Uint16)'9' ) )
            {
                //Append the character
                str += (char)event.key.keysym.unicode;
            }
            //If the key is a uppercase letter
            else if( ( event.key.keysym.unicode >= (Uint16)'A' ) && ( event.key.keysym.unicode <= (Uint16)'Z' ) )
            {
                //Append the character
                str += (char)event.key.keysym.unicode;
            }
            //If the key is a lowercase letter
            else if( ( event.key.keysym.unicode >= (Uint16)'a' ) && ( event.key.keysym.unicode <= (Uint16)'z' ) )
            {
                //Append the character
                str += (char)event.key.keysym.unicode;
            }
        }
             //If backspace was pressed and the string isn't blank
        if( ( event.key.keysym.sym == SDLK_BACKSPACE ) && ( str.length() != 0 ) )
        {
            //Remove a character from the end
            str.erase( str.length() - 1 );
        }
          //If the string was changed
        if( str != temp )
        {
            //Free the old surface
            SDL_FreeSurface( text );

            //Render a new text surface
            text = TTF_RenderText_Solid( font, str.c_str(), white );
        }

    }
}

void StringInput::record()
{

   // convert now to string form
     ofstream fout;
time_t now = time(0);
   // convert now to string form
char* dt = ctime(&now);
        fout.open("record.txt");
fout<<str;

     fout.close();

}
void StringInput::show_centered()
{
    //If the surface isn't blank
    if( text != NULL )
    {
        //Show the name
        apply_surface( ( SCREEN_WIDTH - text->w ) / 2, ( SCREEN_HEIGHT - text->h ) / 2, text, screen );
    }
}
void gameoverdisplay(int score)
{
SDL_Surface* input_disp = NULL;
SDL_Surface* score_disp = NULL;
    bool nameEntered = false;
	valueentered = true;
    StringInput name;
    input_disp = TTF_RenderText_Solid( font_others, "please Enter Name:",white );
    score_disp=TTF_RenderText_Solid(font_numbers,inttostr(score).c_str(),red);
 
    //Apply the images to the screen
    apply_surface( 0, 0, gameover, screen );
    apply_surface(( SCREEN_WIDTH / 2)+50, ( SCREEN_HEIGHT / 2)+25,score_disp,screen);

    SDL_Flip(screen);
   SDL_Delay(5000);
if(score>=name.sadan())
{
  SDL_FreeSurface(gameover);
apply_surface(0,0,background,screen);
apply_surface( ( SCREEN_WIDTH - input_disp->w ) / 2, ( ( SCREEN_HEIGHT / 2 ) - input_disp->h ) / 2, input_disp, screen );
 //Update the screen
 if( SDL_Flip( screen ) == -1 )
    {
        return;
    }
        while( SDL_PollEvent( &event ) )
 	{
            if(( nameEntered == false )&&(valueentered==true))
            {
	        
               //Get user input
                name.handle_input();
                //If the enter key was pressed
                if( ( event.type == SDL_KEYDOWN ) && ( event.key.keysym.sym == SDLK_RETURN ) )
                	{
                    //Change the flag
                    nameEntered = true;
                    name.record();
                    int m=adata(name.sadan(),score);
                    name.recnum(m);
                    //Free the old input surface
                    SDL_FreeSurface( input_disp );

                    //Change the input
                    input_disp= TTF_RenderText_Solid( font_others, "Name Saved", textColor );
                	}
		}
        }

 //Apply the background
        apply_surface( 0, 0, background, screen );

        //Show the input
        apply_surface( ( SCREEN_WIDTH - input_disp->w ) / 2, ( ( SCREEN_HEIGHT / 2 ) - input_disp->h ) / 2, input_disp, screen );
        //Update the screen

        if( SDL_Flip( screen ) == -1 )
        {
            return;
        }

    
}
input_disp = TTF_RenderText_Solid(font_others,"Press Escape to get back to main menu", random_color);
SDL_Flip(screen);
}


bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
//Initialize SDL_ttf
TTF_Init();
TTF_Font *font;
    if( TTF_Init() == -1 )
    {
        return false;    
    }

    //Set the window caption
    SDL_WM_SetCaption( "Do-The-Math", NULL );

    return true;
}

SDL_Surface *load_image( std::string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized image
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old image
        SDL_FreeSurface( loadedImage );
    }

    //Return the optimized image
    return optimizedImage;
}

bool load_files()
{
    //Load the image
    gameover = load_image( "images/gameover.png" );
    background = load_image("images/background.png");
    title = load_image("images/title.png");
    	startbutton = load_image("buttons/play.png");
	startbutton_hover = load_image("buttons/play1.png");
	play_hover = load_image("buttons/play1.png");
    	highscore_hover = load_image("buttons/highscore1.png");
    	howtoplay_hover = load_image("buttons/howtoplay1.png");
    	quit_hover = load_image("buttons/quit1.png");
	blocking = load_image("images/block.png");
	sign_add = load_image("buttons/add.png");
	sign_subtract = load_image("buttons/subtract.png");
	sign_equals = load_image("buttons/equals.png");
	sign_multiply = load_image("buttons/multiply.png");
	sign_divide = load_image("buttons/divide.png");
	puzzle_image[0]=load_image("images_puzzle/image0.png");
	puzzle_image[1]=load_image("images_puzzle/image1.jpg");
	puzzle_image[2]=load_image("images_puzzle/image2.jpg");
	puzzle_image[3]=load_image("images_puzzle/image3.jpg");
	puzzle_image[4]=load_image("images_puzzle/image4.jpg");
	puzzle_image[5]=load_image("images_puzzle/image5.jpg");
	puzzle_image[6]=load_image("images_puzzle/image6.jpg");
	puzzle_image[7]=load_image("images_puzzle/image7.jpg");
	puzzle_image[8]=load_image("images_puzzle/image8.jpg");
	puzzle_image[9]=load_image("images_puzzle/image4.jpg");
	
    //Load the font
	font_numbers = TTF_OpenFont("font.ttf",70);
    font_others = TTF_OpenFont( "font.ttf", 36 );
	font_howtoplay = TTF_OpenFont("font.ttf",24);
	//Load the audio
    high = Mix_LoadWAV( "high.wav" );
    return true;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
    //Temporary rectangle to hold the offsets
    SDL_Rect offset;

    //Get the offsets
    offset.x = x;
    offset.y = y;

    //Blit the surface
    SDL_BlitSurface( source, NULL, destination, &offset );
}
void clean_up()
{
	TTF_CloseFont(font_numbers);
	TTF_CloseFont(font_others);
	TTF_Quit();
	 Mix_FreeChunk( high );
	   Mix_CloseAudio();

    //Quit SDL
    SDL_Quit();
}

int audio()
{
    if( Mix_PlayChannel( -1, high, 0 ) == -1 )
                    {
                        return 1;
                    }
}

void load_buttons()
{
Uint32 colorkey = SDL_MapRGB( background->format, 255, 255, 255 );
SDL_SetColorKey( background, SDL_SRCCOLORKEY, colorkey);
apply_surface(0,0,background,screen);
Uint32 blackcolorkey = SDL_MapRGB(title->format,0,0,0);
SDL_SetColorKey(title,SDL_SRCCOLORKEY,blackcolorkey);	
apply_surface(0,0,title,screen);

}

void load_buttons2()
{
int i = 125, j = 100,x; 
SDL_Delay(1000);
SDL_Flip(screen);
Uint32 blackcolorkey = SDL_MapRGB(title->format,0,0,0);
SDL_SetColorKey(title,SDL_SRCCOLORKEY,blackcolorkey);	

apply_surface(0,0,title,screen);
SDL_Flip(screen);
SDL_Delay(1000);
begin = TTF_RenderText_Solid(font_others,"Enter your answer, and hit Enter.",random_color);
SDL_Flip(screen);
apply_surface(570,i,begin,screen);
begin = TTF_RenderText_Solid(font_others,"Score : ", random_color);
apply_surface(570,(i-100),begin,screen);
SDL_Flip(screen);
SDL_Delay(500);

}
int main( int argc, char* args[] )
{
 //Initialize
    if( init() == false )
    {
        return 1;
    }

    //Load the files
    if( load_files() == false )
    {
        return 1;
    }
  
 
    load_buttons();
  //screen check
    if( SDL_Flip( screen ) == -1 )
    {
        return 1;
   
    }
/*getting keypress event*/ Uint8 *keystates = SDL_GetKeyState( NULL );
    //While the user hasn't quit
    while( quitcheck == false )
    {

        //While there's an event to handle
        while( SDL_PollEvent( &event ) )
        {
if (event.type == SDL_QUIT)
quitcheck = true;

if ((event.type == SDL_MOUSEBUTTONDOWN)&&(event.button.button == SDL_BUTTON_LEFT))	//for button click
	handle_events();
else if ((mainmenu==false)&&(event.type=SDL_MOUSEMOTION))	//main menu still displayed then
motionevents(0);
if ((event.type == SDL_KEYDOWN)&&(mainmenu==true))
{
{
if ((valueentered==false)&&(num<10))
{
SDL_Surface* temp = NULL;
if (keystates[SDLK_BACKSPACE])
{
	num=0;
	apply_surface((SCREEN_WIDTH-250), 250, blocking, screen);
	audio();
}

if (keystates[SDLK_1])
{
	num =(num*10)+1;
	audio();
}
if (keystates[SDLK_2])
{
	num=(num*10)+2;
	audio();
}
if (keystates[SDLK_3])
{
	num=(num*10)+3;
	audio();
}
if (keystates[SDLK_4])
{
	num=(num*10)+4;
	audio();
}
if (keystates[SDLK_5])
{
	num=(num*10)+5;
	audio();
}
if (keystates[SDLK_6])
{	
	num=(num*10)+6;
	audio();
}
if (keystates[SDLK_7])
{
	num=(num*10)+7;
	audio();
}
if (keystates[SDLK_8])
{
	num=(num*10)+8;
	audio();
}
if (keystates[SDLK_9])
{
	num=(num*10)+9;
	audio();
}
if (keystates[SDLK_0])
{
	num=(num*10)+0;
}
if (num!=0)
{
temp = TTF_RenderText_Solid(font_numbers,inttostr(num).c_str(),wait_title_text);
apply_surface((SCREEN_WIDTH-250), 250, temp, screen);
}	
SDL_Flip(screen);

}
if ((keystates[SDLK_RETURN])&&(num!=0))
	{
	valueentered=true;
	checkcorrect(inttostr(num).c_str());
	}

}
}
//Pressing escape will load the main menu
if (keystates[SDLK_ESCAPE])
{
	mainmenu=false;
	clickstart=false;
	background = load_image("images/background.png");
	load_buttons();
}
//Pressing Q will exit the program
if (keystates[SDLK_q])
quitcheck = true;
        }
    }
    //Free the surface and quit SDL
SDL_EnableUNICODE( SDL_DISABLE );

clean_up();
return 0;	
}

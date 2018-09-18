#include<ncurses.h>
#include<unistd.h>
#include<time.h>
#include<stdlib.h>

class snake                         //to store the x and y coordinates of each snake part
{
    int x, y;
    char ch;
    public:
    snake(){ x= y= 0; ch='O';}
    snake(int a, int b)
    {
        x= a; y= b; ch= 'O';
    }
    snake(const snake &ekans)
    {
        ch= ekans.ch;
        x= ekans.x;
        y= ekans.y;
    }
    void setCh(char x)
    {
        ch= x;
    }
    char getCh()
    {
        return ch;
    }
    int getX()
    {
        return x;
    }
    void setX(int no)
    {
        x= no;
    }
    int getY()
    {
        return y;
    }
    void setY(int no)
    {
         y= no;
    }
};

class list                          //to store each snake body          
{
    node* head;
    int length;
    public:
    list()
    {
        head= NULL;
        length= 0;
    }
    void add(snake s)                   //adds at the end
    {
        node *n= new node;
        n->setSnake(s);
        n->setNext(head);
        head= n;
        length++;
    }
    int listLength()
    {
        return length;
    }
    snake get(int n)
    {
        node *temp =head;
        int count= 1;
        while(count!= n && temp!= NULL)
        { 
            count++;
            temp= temp->getNext();
        } 
        return temp->getSnake();
   }
   void remove()                            //removes the first element
   {  
        node *temp= head->getNext();
        node *t2= head;
        while(temp->getNext()!=NULL)
        {
            temp=temp->getNext();
            t2= t2->getNext();
        }
        t2->setNext(NULL);
        delete temp;
        length--;
   }
   void display()
   {
        int i= 0;
        node *temp= head;
        while(temp!= NULL)
        {
            mvaddch(10,10+i,temp->getSnake().getCh());
            mvprintw(20,5,"THIS LOVE");
            refresh();
            temp= temp->getNext();
        }
   }
   ~list()
   {
        while(head!= NULL)
        {
            node* n= head;
            head= head->getNext();
            delete n;
        }
        length= 0;
    }
};

class game
{   
    int score, max_y, max_x, food_x, food_y, direction;
    list l;
    public:
    game()
    {
        score= max_y= max_x= food_x= food_y= 0;
        direction= 2;
    }
    void launch();
    void play();
    void map();
    void genFood();
    void setSnake();
    void moveSnake(int dir);
    bool check();
};

void game:: launch()
{
    initscr();                      //initialize ncurses
    cbreak();                       //no line buffering
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    noecho();
    refresh();
    attron(A_BOLD);
    mvprintw(LINES/2-2, COLS/2-10, "SNAKES WELCOMES YOU");
    mvprintw(LINES/2, COLS/2-12, "PRESS ENTER TO CONTINUE");
    int ch;
    if((ch= getch())==10)
    {
        getmaxyx(stdscr, max_y, max_x);
        play();
    }
    else 
    {
        clear();
        mvprintw(LINES/2, COLS/2- 16, "YOU DID NOT EVEN GIVE ME A CHANCE......GOODBYE");
        refresh();
        sleep(2);
    }
    attroff(A_BOLD);
    endwin();
}

void game:: genFood()
{
    srand(time(NULL));
    food_y= random()%(max_y-6)+4;
    food_x= random()%(max_x-4)+2;
}

void game:: map()
{
    box(stdscr, 0, 0);
    mvprintw(1,1, "SCORE: ");
    mvprintw(1,8, "%d",score);      
    mvprintw(2,1, "Press q to quit");
    refresh();
}

void game:: setSnake()
{
    clear();
    map();
    for(int i= 0; i< 8; i++)
    {
        snake s((COLS/2)-8+i, LINES/2);
        l.add(s);
        mvprintw(s.getY(),s.getX(),"%c",s.getCh());
    }
    mvprintw(food_y, food_x, "F");
    refresh();
}

void game::play()
{
    bool b;
    genFood();
    setSnake();
    mvprintw(8, COLS/2-20, "??????????READY??????????????");
    refresh();
    sleep(1);
    int ch= 0;
    timeout(50);                            //adjust speed of the game
    while((ch=getch())!= 'q')
    {
        switch(ch)
        {
            case KEY_UP: direction= 1; 
                        break;
            case KEY_DOWN: direction= 3;
                        break;
            case KEY_RIGHT: direction= 2;
                        break;
            case KEY_LEFT: direction= 4;
                        break;
        }
       clear();
       map();
       mvprintw(food_y, food_x,"F");
       moveSnake(direction);
       refresh();
       b= check();
       if(!b)
       {
            clear();
            mvprintw(max_y/2-2, max_x/2-8, "GAME OVER");
            mvprintw(max_y/2, max_x/2-10, "YOUR SCORE %d",score);
            refresh();
            sleep(2);
            break;
        }
    }
}

bool game:: check()
{
    bool b= false;
    snake s= l.get(1);
    int head_x= s.getX();
    int head_y= s.getY();
    if(head_x== max_x-1 || head_y== 1 || head_x== 1 || head_y== max_y-1)
        return false;
    else 
    {
        int len= l.listLength();
        for(int i= 4; i< len; i++)
        {
            s= l.get(i);
            if(head_x== s.getX() && head_y== s.getY())
            {
                b= true;
                break;
            }
        }
        if(b)
            return false;
        else
            return true;
    }  
}

void game:: moveSnake(int dir)
{
    snake k;
    k= l.get(1);
    int x= k.getX(), y= k.getY();
    //mvprintw(6,1, "BEFORE x= %d, y= %d, dir=%d",x,y,dir);
    if(dir== 1) y--;
    else if(dir== 2)    x++;
    else if(dir== 3)    y++;
    else x--;
    //mvprintw(7,1, "AFTER x=%d, y= %d", x, y);
    snake s(x,y);
    l.add(s);
    if(x==food_x && y==food_y)
    {
        score++;
        genFood();
        mvprintw(food_y, food_x, "F");
    }
    else
        l.remove();
    int len= l.listLength();
    for(int i= 1; i<= len; i++)
    {
        k= l.get(i);
        mvaddch(k.getY(), k.getX(), k.getCh());
        refresh();
    }
}

int main()
{
    game g;
    g.launch();
    return 0;
} 
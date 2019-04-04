#include "picosheader.h"
#include <gtk/gtk.h>
#include <cairo.h>

static GtkWidget *darea2,*darea22;
static GtkWidget *darea,*dareaS,*dareaS2;
static GtkWidget *window;
static GtkWidget *table;
static GtkWidget *freqValues;
static GtkWidget *adcValues;
static GtkWidget *fdevValues;
static GtkWidget *sdevValues;
static GtkWidget *mfi1Values,*mff1Values,*mfi2Values,*mff2Values;

GError *error = NULL;
int readADC[50];
int readFDEV[49];
int readSDEV[48];	
char frequency[32];
char vpp[32];
char vmax[32];
char vmin[32];

guint tout=200;
const double redzoneborder=275;
double xaxis=9.0;

gboolean decide = TRUE;
gboolean toggledebug = FALSE;

static gboolean scrolled(GtkWidget *widget , GdkEventScroll *event,gpointer user_data);
static void do_signal(cairo_t *); 
static void do_signal2(cairo_t *); 
static gboolean do_signal_event(GtkWidget * , cairo_t * ,gpointer ); 
static gboolean do_signal_event2(GtkWidget * , cairo_t * ,gpointer ); 
static void do_drawing(cairo_t *);
static void do_debug_draw(cairo_t *);
static gboolean on_draw_event(GtkWidget * , cairo_t *, gpointer); 
static gboolean on_stop(GtkWidget *); 

static gboolean on_delete_event();
static gboolean on_show_event();
static gboolean on_delete_event2();
static gboolean on_show_event2();

static gboolean on_deletedebug_event();
static gboolean on_showdebug_event();
static gboolean on_deletedebug_event2();
static gboolean on_showdebug_event2();

static gboolean on_start(GtkWidget *); 
static gboolean on_debugdraw_event(GtkWidget *widget , cairo_t *cr, gpointer user_data); 
static void switcher();


static void switcher() {
	if(decide == TRUE){
		decide=FALSE;
		on_delete_event2();
		on_show_event();
		/*if(toggledebug == TRUE) {
			on_deletedebug_event2();
			on_showdebug_event();
		}*/
	} else if(decide==FALSE) {
		decide=TRUE;
		on_delete_event();
		on_show_event2();
		/*if(toggledebug == TRUE) {
			on_deletedebug_event();
			on_showdebug_event2();
		}*/
	}
		
}



static gboolean do_signal_event2(GtkWidget *widget , cairo_t *cr ,gpointer user_data) {

	do_signal2(cr);
	
	gtk_widget_queue_draw(widget);
	return FALSE;
}

static gboolean do_signal_event(GtkWidget *widget , cairo_t *cr ,gpointer user_data) {

	do_signal(cr);
	
	gtk_widget_queue_draw(widget);
	return FALSE;
}

//GTK funtion to draw signal
static gboolean on_draw_event(GtkWidget *widget , cairo_t *cr, gpointer user_data) {
	do_drawing(cr);
	gtk_widget_queue_draw(widget);
	return FALSE;
}

//GTK function to hide debug plot widget
static gboolean on_stop(GtkWidget *w) {
	
	gtk_widget_hide(w);
	return FALSE;
}
//GTK funtion to show debug plot
static gboolean on_start(GtkWidget *w) {

	gtk_widget_show(w);
	gtk_widget_hide(dareaS2);
}

//GTK funtion to hide signal
static gboolean on_delete_event() {
	gtk_widget_hide(dareaS);
}

//GTK funtion to show signal
static gboolean on_show_event() {

	
	gtk_widget_show(dareaS);

}

//GTK funtion to hide signal
static gboolean on_delete_event2(gpointer *w) {
	gtk_widget_hide(dareaS2);
}

//GTK funtion to show signal
static gboolean on_show_event2(gpointer *w) {

	
	gtk_widget_show(dareaS2);

}

//GTK funtion to hide signal
static gboolean on_deletedebug_event() {
	gtk_widget_hide(darea2);
}

//GTK funtion to show signal
static gboolean on_showdebug_event() {

	
	gtk_widget_show(darea2);

}

//GTK funtion to hide signal
static gboolean on_deletedebug_event2(gpointer *w) {
	gtk_widget_hide(darea22);
}

//GTK funtion to show signal
static gboolean on_showdebug_event2(gpointer *w) {

	
	gtk_widget_show(darea22);

}

//GTK function to call cairo to draw signal
static gboolean on_debugdraw_event(GtkWidget *widget , cairo_t *cr, gpointer user_data) {
	do_debug_draw(cr);
	return FALSE;
}

//Cairo funtion to draw debug plot
static void do_debug_draw(cairo_t *cr) {

	int i;

	//first derivative plot
	cairo_set_source_rgb(cr,0,150,150);
	cairo_set_line_width(cr,2);
	cairo_move_to(cr,0,125);	

	int horzflag=0;
	//while((horzflag*xaxis)>=redzoneborder) {
	for(i=0;i<49;i++) {
		if((horzflag*xaxis)>=redzoneborder) break;
		cairo_move_to(cr,horzflag*xaxis,(103-(readFDEV[i]*10)+12));
		cairo_line_to(cr,(horzflag+1)*xaxis,(103-(readFDEV[i+1]*10)+12));
		horzflag++;
	}
	cairo_set_line_width(cr,2);
	cairo_stroke(cr);
	//}

	//second derivative plot
	cairo_set_source_rgb(cr,150,0,0);
	cairo_set_line_width(cr,2);
	cairo_move_to(cr,0,0);
	horzflag=0;
	//while((horzflag*xaxis)>=redzoneborder) {
	for(i=0;i<47;i++) {
		if((horzflag*xaxis)>=redzoneborder) break;
		cairo_move_to(cr,horzflag*xaxis,(103-(readSDEV[i]*10)+12));
		cairo_line_to(cr,(horzflag+1)*xaxis,(103-(readSDEV[i+1]*10)+12));
		horzflag++;
	}

	cairo_set_line_width(cr,2);
	cairo_stroke(cr);
	//}
	//Index First derivative
	cairo_set_line_width(cr,5);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_font_size(cr, 10);
	cairo_move_to(cr, 0, 150);
	cairo_show_text(cr, "First Derivative of signal");

	cairo_set_source_rgb(cr,0,150,150);
	cairo_set_line_width(cr,3);
	cairo_move_to(cr, 150 , 147);
	cairo_line_to(cr,200,147);
	cairo_stroke(cr);

	//Index Second Derivative	
	cairo_set_line_width(cr,5);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_font_size(cr, 10);
	cairo_move_to(cr, 0, 170);
	cairo_show_text(cr, "Second Derivative of Signal");

	cairo_set_source_rgb(cr,150,0,0);
	cairo_set_line_width(cr,3);
	cairo_move_to(cr,150,167);
	cairo_line_to(cr,200,167);
	cairo_stroke(cr);
	
}

static void do_debug_draw2(cairo_t *cr) {

	int i;
	int horzflag=5;

	//first derivative plot
	cairo_set_source_rgb(cr,0,150,150);
	cairo_set_line_width(cr,2);
	cairo_move_to(cr,0,125);	

	//while((horzflag*xaxis)>=redzoneborder) {
	for(i=5;i<54;i++) {
		if((horzflag*xaxis)>=redzoneborder) break;
		cairo_move_to(cr,horzflag*xaxis,(103-(readFDEV[i-5]*10)+12));
		cairo_line_to(cr,(horzflag+1)*xaxis,(103-(readFDEV[i-4]*10)+12));
		horzflag++;
	}
	cairo_set_line_width(cr,2);
	cairo_stroke(cr);
	//}
	//second derivative plot
	cairo_set_source_rgb(cr,150,0,0);
	cairo_set_line_width(cr,2);
	cairo_move_to(cr,0,0);
	
	horzflag=5;
	
	for(i=5;i<53;i++) {
		if((horzflag*xaxis)>=redzoneborder) break;
		cairo_move_to(cr,horzflag*xaxis,(103-(readSDEV[i-5]*10)+12));
		cairo_line_to(cr,(horzflag+1)*xaxis,(103-(readSDEV[i-4]*10)+12));
		horzflag++;
	}

	cairo_set_line_width(cr,2);
	cairo_stroke(cr);
	

	cairo_set_line_width(cr,2);
	cairo_stroke(cr);
	//Index First derivative
	cairo_set_line_width(cr,5);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_font_size(cr, 10);
	cairo_move_to(cr, 0, 150);
	cairo_show_text(cr, "First Derivative of signal");

	cairo_set_source_rgb(cr,0,150,150);
	cairo_set_line_width(cr,3);
	cairo_move_to(cr, 150 , 147);
	cairo_line_to(cr,200,147);
	cairo_stroke(cr);

	//Index Second Derivative	
	cairo_set_line_width(cr,5);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_font_size(cr, 10);
	cairo_move_to(cr, 0, 170);
	cairo_show_text(cr, "Second Derivative of Signal");

	cairo_set_source_rgb(cr,150,0,0);
	cairo_set_line_width(cr,3);
	cairo_move_to(cr,150,167);
	cairo_line_to(cr,200,167);
	cairo_stroke(cr);
	
}

static void do_signal(cairo_t *cr) {
	//draw signal lines
	cairo_set_source_rgb(cr,0,0,0);
	cairo_set_line_width(cr,2);
	cairo_move_to(cr,0,0);	
	int i,horzflag;
	
	horzflag=0;
	for(i=0;i<49;i++) {
		if((horzflag*xaxis)>=redzoneborder) break;
		cairo_move_to(cr,horzflag*xaxis,(103-readADC[i]/10)+12);
		cairo_line_to(cr,(horzflag+1)*xaxis,(103-readADC[i+1]/10)+12);
		horzflag++;
	}
	cairo_set_line_width(cr,2);
	cairo_stroke(cr);
/*	horzflag=49;
	for(i=49;i<54;i++) {
		if((horzflag*xaxis)>=redzoneborder) break;
		cairo_move_to(cr,horzflag*xaxis,(103-readADC[i-49]/10)+12);
		cairo_line_to(cr,(horzflag+1)*xaxis,(103-readADC[i-48]/10)+12);
		horzflag++;
	}
	cairo_set_line_width(cr,2);
	cairo_stroke(cr);*/

}

static void do_signal2(cairo_t *cr) {
	//draw signal lines
	cairo_set_source_rgb(cr,0,0,0);
	cairo_set_line_width(cr,2);
	int i;
	int horzflag=0;
	
	for(i=0;i<5;i++) {
		if((horzflag*xaxis)>=redzoneborder) break;
		cairo_move_to(cr,horzflag*xaxis,(103-readADC[49-i]/10)+12);
		cairo_line_to(cr,(horzflag+1)*xaxis,(103-readADC[48-i]/10)+12);
		horzflag++;
	}
	cairo_set_line_width(cr,2);
	cairo_stroke(cr);

	horzflag=5;
	for(i=5;i<54;i++) {
		
		if((horzflag*xaxis)>=redzoneborder) break;
		cairo_move_to(cr,horzflag*xaxis,(103-readADC[i-5]/10)+12);
		cairo_line_to(cr,(horzflag+1)*xaxis,(103-readADC[i-4]/10)+12);
		horzflag++;
	}
	cairo_set_line_width(cr,2);
	cairo_stroke(cr);
	
}

//Cairo Funtion to draw the area
static void do_drawing(cairo_t *cr) {

	// title of signal
	cairo_set_line_width(cr,5);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_font_size(cr, 10);
	cairo_move_to(cr, 0, 10);
	cairo_show_text(cr, "Signal");
	//To show Voltage value index
	cairo_set_line_width(cr,5);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_font_size(cr, 10);
	cairo_move_to(cr, 285, 15);
	cairo_show_text(cr, "-  5V");

	cairo_set_line_width(cr,5);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_font_size(cr, 10);
	cairo_move_to(cr, 285, 35);
	cairo_show_text(cr, "-  4V");

	cairo_set_line_width(cr,5);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_font_size(cr, 10);
	cairo_move_to(cr, 285, 55);
	cairo_show_text(cr, "-  3V");

	cairo_set_line_width(cr,5);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_font_size(cr, 10);
	cairo_move_to(cr, 285, 75);
	cairo_show_text(cr, "-  2V");

	cairo_set_line_width(cr,5);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_font_size(cr, 10);
	cairo_move_to(cr, 285, 95);
	cairo_show_text(cr, "-  1V");

	cairo_set_line_width(cr,5);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_font_size(cr, 10);
	cairo_move_to(cr, 285, 115);
	cairo_show_text(cr, "-  0V");

	
	
	//To show Frequency,Vpp,Vmin,Vmax
	cairo_set_line_width(cr,5);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_font_size(cr, 10);
	cairo_move_to(cr, 0, 130);
	cairo_show_text(cr, "FREQUENCY :");
	
	cairo_move_to(cr, 80, 130);
	cairo_show_text(cr, frequency);

	cairo_stroke(cr);


	cairo_set_line_width(cr,5);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_font_size(cr, 10);
	cairo_move_to(cr, 130, 130);
	cairo_show_text(cr, "Vpp :");
	
	cairo_move_to(cr, 150, 130);
	cairo_show_text(cr, vpp);

	cairo_stroke(cr);


	cairo_set_line_width(cr,5);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_font_size(cr, 10);
	cairo_move_to(cr, 200, 130);
	cairo_show_text(cr, "Vmax :");
	
	cairo_move_to(cr, 230, 130);
	cairo_show_text(cr, vmax);

	cairo_stroke(cr);


	cairo_set_line_width(cr,5);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_font_size(cr, 10);
	cairo_move_to(cr, 270, 130);
	cairo_show_text(cr, "Vmin :");
	
	cairo_move_to(cr, 300, 130);
	cairo_show_text(cr, vmin);

	cairo_stroke(cr);

	//inner red rectangle
	cairo_rectangle(cr,0,11,280,103);
	cairo_set_source_rgba (cr,1,0,0,0.5);
	cairo_fill(cr);
	//outer gray rectangle
	cairo_rectangle(cr,0,0,400,180);
	cairo_set_source_rgba (cr,0,0,0,0.1);
	cairo_fill(cr);



}

//function to assign application icon
GdkPixbuf *create_pixbuf(const gchar * filename) {
    
   GdkPixbuf *pixbuf;
   GError *error = NULL;
   pixbuf = gdk_pixbuf_new_from_file(filename, &error);
   
   if (!pixbuf) {
       
      fprintf(stderr, "%s\n", error->message);
      g_error_free(error);
   }

   return pixbuf;
}

/*void get_screenshot(){
    GdkPixbuf *screenshot;

    screenshot = gdk_pixbuf_get_from_window (GTK_WINDOW(window), 400,400,100,100);
    gdk_pixbuf_save(GDK_PIXBUF(screenshot),"jeguester","jpeg",&error,"quality","100",NULL);

                                        
}*/	
GdkPixbuf * get_screenshot(){
    GdkPixbuf *screenshot;
    GdkWindow *root_window;
    gint x_orig, y_orig;
    gint width, height;
    root_window = gdk_get_default_root_window ();
    gtk_table_get_size (root_window, &width, &height);
    gdk_window_get_origin (root_window, &x_orig, &y_orig);

    screenshot = gdk_pixbuf_get_from_surface (table, width,height,100,100);
                                        
    gdk_pixbuf_save(GDK_PIXBUF(screenshot),"jeguester","jpeg",&error,"quality","100",NULL);
    return screenshot;
}

//Thread to communicate with PIC and to do calculations 
static gpointer doPic(gpointer data) {
	int x=0;
	struct rets *returnvalues=(struct rets *)malloc(sizeof(struct rets));
	confport();
	openport();	
	while(1) {
		write(fd,"g",1);
		int *m=portreadN();
		returnvalues=compose(m);
		
		snprintf(frequency,sizeof(frequency)," %d Hz",returnvalues->freqint);		
		snprintf(vpp,sizeof(vpp),"  %d V",returnvalues->vpp);		
		snprintf(vmin,sizeof(vmin),"  %d V",returnvalues->vmin);		
		snprintf(vmax,sizeof(vmax),"  %d V",returnvalues->vmax);		
		
		x++;
                printf("\n%d kez girdi\n",x);



		for(int i=0;i<50;i++){
			readADC[i]=returnvalues->adc[i];
			printf("\n%d --> adc: %d",i,readADC[i]);
			if(i<49){ readFDEV[i]=returnvalues->fdev[i];
			printf("\t\t%d --> fdev: %d",i,readFDEV[i]);
			}
			if(i<48) {readSDEV[i]=returnvalues->sdev[i];
			printf("\t\t\t\t%d --> sdev: %d",i,readSDEV[i]);
			}
		}
	
	}	
	int c=close(fd);

	if (c==-1) {
               fprintf(stderr,"error = %s\n",strerror(errno));
               exit(-1);
              }
	return NULL;
}

static gboolean scrolled(GtkWidget *widget , GdkEventScroll *event,gpointer user_data) {

	if (event->direction == GDK_SCROLL_UP) {
       		xaxis=xaxis+0.1;
        	gtk_widget_queue_draw(widget);
    	}

    	if (event->direction == GDK_SCROLL_DOWN) {
	
        	xaxis=xaxis-0.1;
        	gtk_widget_queue_draw(widget);
    }

    return TRUE;

}


int main(int argc, char **argv) {
//       	GtkWidget *window, *grid;
	GdkPixbuf *icon,*screenshot;
    	GtkWidget *button, *button1;
    	GThread   *thread;


    	if( ! g_thread_supported() )
        	g_thread_init( NULL );

	gtk_init(&argc, &argv);


	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW (window) , "Jeguester Scope");
	gtk_window_set_default_size(GTK_WINDOW(window), 450,185);
	gtk_container_set_border_width (GTK_CONTAINER (window), 3);	
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_widget_add_events(window,GDK_SCROLL_MASK);

	table=gtk_table_new(4,5,TRUE);
	gtk_container_add (GTK_CONTAINER (window) ,table);
	
	darea=gtk_drawing_area_new();
	g_signal_connect(G_OBJECT(darea),"draw",G_CALLBACK(on_draw_event),NULL);
	gtk_table_attach_defaults(GTK_TABLE(table),darea,0,4,0,4);	

	dareaS=gtk_drawing_area_new();
	g_signal_connect(G_OBJECT(dareaS),"draw",G_CALLBACK(do_signal_event),NULL);
	gtk_table_attach_defaults(GTK_TABLE(table),dareaS,0,4,0,4);	


	dareaS2=gtk_drawing_area_new();
	g_signal_connect(G_OBJECT(dareaS2),"draw",G_CALLBACK(do_signal_event2),NULL);
	gtk_table_attach_defaults(GTK_TABLE(table),dareaS2,0,4,0,4);	
	
	g_timeout_add(tout,switcher,NULL);

	g_signal_connect(window,"scroll-event",G_CALLBACK(scrolled),NULL);


	darea2=gtk_drawing_area_new();
	g_signal_connect(G_OBJECT(darea2),"draw",G_CALLBACK(on_debugdraw_event),NULL);
	gtk_table_attach_defaults(GTK_TABLE(table),darea2,0,4,0,4);

	darea22=gtk_drawing_area_new();
	g_signal_connect(G_OBJECT(darea22),"draw",G_CALLBACK(on_debugdraw_event),NULL);
	gtk_table_attach_defaults(GTK_TABLE(table),darea22,0,4,0,4);

	icon=create_pixbuf("analog-icon-17.jpg");
	gtk_window_set_icon(GTK_WINDOW(window),icon);


    	thread = g_thread_create( doPic, (gpointer)button,FALSE, &error );
    	if( ! thread )
    	{
        	g_print( "Error: %s\n", error->message );
        	return( -1 );
    	}


	
	
	button1 = gtk_button_new_with_label ("Show\nDebugger");
        g_signal_connect_swapped (button1, "clicked", G_CALLBACK(on_start), darea2);
       	gtk_table_attach_defaults (GTK_TABLE (table), button1, 4,5,0,1);

	button = gtk_button_new_with_label ("Hide\nDebugger");
        g_signal_connect_swapped (button, "clicked", G_CALLBACK(on_stop), darea2);
       	gtk_table_attach_defaults (GTK_TABLE (table), button, 4,5,1,2);

	button = gtk_button_new_with_label ("Get\nSnapshot");
        g_signal_connect_swapped (button, "clicked", G_CALLBACK(get_screenshot), NULL);
       	gtk_table_attach_defaults (GTK_TABLE (table), button, 4,5,2,3);

	gtk_widget_show_all(window);

	gtk_widget_hide(darea2);
	gtk_widget_hide(darea22);
	g_object_unref(icon);
	gtk_main();

    return 0;
}

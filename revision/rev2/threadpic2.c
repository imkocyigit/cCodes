#include "picosheader.h"
#include <gtk/gtk.h>
#include <cairo.h>
gint progress = 1;
G_LOCK_DEFINE_STATIC( progress );


static GtkWidget *freqValues;
static GtkWidget *adcValues;
static GtkWidget *fdevValues;
static GtkWidget *sdevValues;
static GtkWidget *mfi1Values,*mff1Values,*mfi2Values,*mff2Values;

int readADC[50];
int readFDEV[49];
int readSDEV[48];	
char frequency[32];

static void do_drawing(cairo_t *);
static void do_debug_draw(cairo_t *);

static gboolean on_draw_event(GtkWidget *widget , cairo_t *cr, gpointer user_data) {
	do_drawing(cr);
	gtk_widget_queue_draw(widget);
	return FALSE;
}

static gboolean on_debugdraw_event(GtkWidget *widget , cairo_t *cr, gpointer user_data) {
	do_debug_draw(cr);
	gtk_widget_queue_draw(widget);
	return FALSE;
}

static void do_debug_draw(cairo_t *cr) {


	cairo_set_source_rgb(cr,0,150,150);
	cairo_set_line_width(cr,2);
	cairo_move_to(cr,0,125);	

	for(i=0;i<49;i++) {
		cairo_move_to(cr,i*5,(readFDEV[i]*10)+145);
		cairo_line_to(cr,(i+1)*5,(readFDEV[i+1]*10)+145);
	}
	cairo_set_line_width(cr,2);
	cairo_stroke(cr);

	cairo_set_source_rgb(cr,150,0,0);
	cairo_set_line_width(cr,2);
	cairo_move_to(cr,0,0);

	for(i=0;i<47;i++) {
		cairo_move_to(cr,i*5,(readSDEV[i]*10)+275);
		cairo_line_to(cr,(i+1)*5,(readSDEV[i+1]*10)+275);
	}

	cairo_set_line_width(cr,2);
	cairo_stroke(cr);

	cairo_set_line_width(cr,5);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_font_size(cr, 10);
	cairo_move_to(cr, 0, 140);
	cairo_show_text(cr, "First Derivative of signal");
	
	cairo_set_line_width(cr,5);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_font_size(cr, 10);
	cairo_move_to(cr, 0, 255);
	cairo_show_text(cr, "Second Derivative of Signal");


	cairo_rectangle(cr,0,145,300,103);
	cairo_set_source_rgba (cr,1,1,0,0.1);
	cairo_fill(cr);

	cairo_rectangle(cr,0,265,300,103);
	cairo_set_source_rgba (cr,150,0,1,0.1);

	cairo_fill(cr);


}

static void do_drawing(cairo_t *cr) {
	cairo_set_source_rgb(cr,0,0,0);
	cairo_set_line_width(cr,2);
	cairo_move_to(cr,0,0);	
	int i;
	for(i=0;i<49;i++) {
		cairo_move_to(cr,i*5,(readADC[i]/10)+12);
		cairo_line_to(cr,(i+1)*5,(readADC[i+1]/10)+12);
	}
	cairo_set_line_width(cr,2);
	cairo_stroke(cr);

	

	cairo_set_line_width(cr,5);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_font_size(cr, 10);
	cairo_move_to(cr, 0, 10);
	cairo_show_text(cr, "Signal");

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

	


	cairo_set_line_width(cr,5);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_font_size(cr, 10);
	cairo_move_to(cr, 0, 450);
	cairo_show_text(cr, "FREQUENCY :");
	
	cairo_move_to(cr, 80, 450);
	cairo_show_text(cr, frequency);

	cairo_stroke(cr);

	cairo_rectangle(cr,0,11,280,103);
	cairo_set_source_rgba (cr,1,0,0,0.1);
	cairo_fill(cr);

}

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

/*GdkPixbuf * get_screenshot(){
    GdkPixbuf *screenshot;
    GdkWindow *root_window;
    gint x_orig, y_orig;
    gint width, height;
    root_window = gdk_get_default_root_window ();
    gdk_drawable_get_size (root_window, &width, &height);      
    gdk_window_get_origin (root_window, &x_orig, &y_orig);

    screenshot = gdk_pixbuf_get_from_drawable (NULL, root_window, NULL,
                                           x_orig, y_orig, 0, 0, width, height);
    return screenshot;
}*/	


static gpointer doPic(gpointer data) {
	int x=0;
	struct rets *returnvalues=(struct rets *)malloc(sizeof(struct rets));
	confport();
	openport();	
	while(1) {
		write(fd,"g",1);
		int *m=portreadN();
		snprintf(frequency,sizeof(frequency),"  %d  Hz",returnvalues->freqint);		
		returnvalues=compose(m);
		x++;
                printf("\n%d kez girdi\n",x);



		for(int i=0;i<50;i++){
			readADC[i]=returnvalues->adc[i];
			printf("\n%d --> adc: %d",i,readADC[i]);
			if(i<49) readFDEV[i]=returnvalues->fdev[i];
			printf("\t\t%d --> fdev: %d",i,readFDEV[i]);
			if(i<48) readSDEV[i]=returnvalues->sdev[i];
			printf("\t\t\t\t%d --> sdev: %d",i,readSDEV[i]);
		}
	
	}	
	int c=close(fd);

	if (c==-1) {
               fprintf(stderr,"error = %s\n",strerror(errno));
               exit(-1);
              }
	return NULL;
}

int main(int argc, char **argv) {
       	GtkWidget *window, *grid;
	GdkPixbuf *icon,*screenshot;
	GtkWidget *darea,*darea2;
    	GtkWidget *button;
	GtkWidget *table;
    	GThread   *thread;
    	GError    *error = NULL;


    	if( ! g_thread_supported() )
        	g_thread_init( NULL );

	gtk_init(&argc, &argv);



	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW (window) , "PICOSCOPE");
	gtk_window_set_default_size(GTK_WINDOW(window), 450,450);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	table=gtk_table_new(15,10,TRUE);
	gtk_container_add (GTK_CONTAINER (window) ,table);
	
	darea2=gtk_drawing_area_new();
	g_signal_connect(G_OBJECT(darea2),"draw",G_CALLBACK(on_debugdraw_event),NULL);
	gtk_table_attach_defaults(GTK_TABLE(table),darea2,0,6,15,30);


	darea=gtk_drawing_area_new();
	g_signal_connect(G_OBJECT(darea),"draw",G_CALLBACK(on_draw_event),NULL);
	gtk_table_attach_defaults(GTK_TABLE(table),darea,0,6,0,15);	
		
	icon=create_pixbuf("analog-icon-17.jpg");
	gtk_window_set_icon(GTK_WINDOW(window),icon);


    	thread = g_thread_create( doPic, (gpointer)button,FALSE, &error );
    	if( ! thread )
    	{
        	g_print( "Error: %s\n", error->message );
        	return( -1 );
    	}



	button = gtk_button_new_with_label ("Quit");
        g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_widget_destroy), window);
       	gtk_table_attach_defaults (GTK_TABLE (table), button, 9,10,9,10);
	
	gtk_widget_show_all(window);
	g_object_unref(icon);
	gtk_main();

    return 0;
}

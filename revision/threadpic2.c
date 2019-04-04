
#include "picos.h"
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

static gboolean on_draw_event(GtkWidget *widget , cairo_t *cr, gpointer user_data) {
	do_drawing(cr);
	gtk_widget_queue_draw(widget);
	return FALSE;
}

static void do_drawing(cairo_t *cr) {
	cairo_set_source_rgb(cr,0,0,0);
	cairo_set_line_width(cr,2);
	cairo_move_to(cr,0,0);	
	int i;
	for(i=0;i<50;i++) {
		cairo_move_to(cr,i*5,(readADC[i]/10)+12);
		cairo_line_to(cr,(i+1)*5,(readADC[i+1]/10)+12);
	}
	cairo_set_line_width(cr,2);
	cairo_stroke(cr);

	cairo_set_source_rgb(cr,0,150,150);
	cairo_set_line_width(cr,2);
	cairo_move_to(cr,0,110);	
	
	for(i=0;i<49;i++) {
		cairo_move_to(cr,i*5,(readFDEV[i]*10)+130);
		cairo_line_to(cr,(i+1)*5,(readFDEV[i+1]*10)+130);
	}
	cairo_set_line_width(cr,2);
	cairo_stroke(cr);

	cairo_set_source_rgb(cr,150,0,0);
	cairo_set_line_width(cr,2);
	cairo_move_to(cr,0,0);

	for(i=0;i<47;i++) {
		cairo_move_to(cr,i*5,(readSDEV[i]*10)+260);
		cairo_line_to(cr,(i+1)*5,(readSDEV[i+1]*10)+260);
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
	cairo_move_to(cr, 0, 125);
	cairo_show_text(cr, "First Derivative of signal");
	
	cairo_set_line_width(cr,5);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_font_size(cr, 10);
	cairo_move_to(cr, 0, 240);
	cairo_show_text(cr, "Second Derivative of Signal");


	cairo_set_line_width(cr,5);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_font_size(cr, 10);
	cairo_move_to(cr, 0, 400);
	cairo_show_text(cr, "FREQUENCY :");
	
	cairo_move_to(cr, 80, 400);
	cairo_show_text(cr, frequency);

	cairo_stroke(cr);

	cairo_rectangle(cr,0,11,300,103);
	cairo_set_source_rgba (cr,1,0,0,0.1);
	cairo_fill(cr);

	cairo_rectangle(cr,0,130,300,103);
	cairo_set_source_rgba (cr,1,1,0,0.1);
	cairo_fill(cr);

	cairo_rectangle(cr,0,250,300,103);
	cairo_set_source_rgba (cr,150,0,1,0.1);

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

static gpointer doPic(gpointer data) {

	struct rets *returnvalues=(struct rets *)malloc(sizeof(struct rets));
	confport();
	openport();	
	while(1) {
		char *m=readport();
		
		printf("port will be read");
		returnvalues=ikea(m);
		snprintf(frequency,sizeof(frequency),"  %d  ",returnvalues->freqint);



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
	GdkPixbuf *icon;
	GtkWidget *darea;
    	GtkWidget *button;
    	GThread   *thread;
    	GError    *error = NULL;


    	if( ! g_thread_supported() )
        	g_thread_init( NULL );

	gtk_init(&argc, &argv);



	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW (window) , "PICOSCOPE");
	gtk_window_set_default_size(GTK_WINDOW(window), 400,400);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	grid = gtk_grid_new();
	gtk_container_add(GTK_CONTAINER(window), grid);

	darea=gtk_drawing_area_new();
	gtk_container_add(GTK_CONTAINER(window),darea);
		
	icon=create_pixbuf("analog-icon-17.jpg");
	gtk_window_set_icon(GTK_WINDOW(window),icon);
		


    	thread = g_thread_create( doPic, (gpointer)button,FALSE, &error );
    	if( ! thread )
    	{
        	g_print( "Error: %s\n", error->message );
        	return( -1 );
    	}

       	freqValues = gtk_label_new("Freq:");
       	gtk_grid_attach(GTK_GRID(grid), freqValues, 0, 1, 5, 5);

       	adcValues = gtk_label_new("Signal:");
       	gtk_grid_attach(GTK_GRID(grid), adcValues, 0,6, 5, 5);

       	fdevValues = gtk_label_new("First Derivative:");
       	gtk_grid_attach(GTK_GRID(grid), fdevValues, 0, 11, 5, 5);

       	sdevValues = gtk_label_new("Second derivative:");
       	gtk_grid_attach(GTK_GRID(grid), sdevValues, 0, 16, 5, 5);

       	mfi1Values = gtk_label_new("Most Frequent index in first derivative:");
       	gtk_grid_attach(GTK_GRID(grid), mfi1Values, 0,21, 5, 5);

       	mff1Values = gtk_label_new("Frequency of most frequent index in first der.:");
       	gtk_grid_attach(GTK_GRID(grid), mff1Values, 0, 26, 5, 5);

       	mfi2Values = gtk_label_new("Most Frequent index in second derivative:");
       	gtk_grid_attach(GTK_GRID(grid), mfi2Values, 0, 31, 5, 5);

       	mff2Values = gtk_label_new("Frequency of most frequent index in second der.");
       	gtk_grid_attach(GTK_GRID(grid), mff2Values, 0, 36, 5, 5);

	g_signal_connect(G_OBJECT(darea),"draw",G_CALLBACK(on_draw_event),NULL);

	button = gtk_button_new_with_label ("Quit");
        g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_widget_destroy), window);
       	gtk_grid_attach (GTK_GRID (grid), button, 50,0, 1, 1);
	
	gtk_widget_show_all(window);
	g_object_unref(icon);
	gtk_main();

    return 0;
}

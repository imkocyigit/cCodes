#include "picos.h"
#include <gtk/gtk.h>
#include <cairo.h>
gint progress = 0;
G_LOCK_DEFINE_STATIC( progress );

static GtkWidget *freqValues;
static GtkWidget *adcValues;
static GtkWidget *fdevValues;
static GtkWidget *sdevValues;
static GtkWidget *mfi1Values,*mff1Values,*mfi2Values,*mff2Values;
static GtkWidget *msgValues;
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
	int readADC[50];
	int readFDEV[49];
	int readSDEV[48];	
	confport();
	openport();	
	while(1) {
		char *m=readport();
		returnvalues=ikea(m);
		for(int i=0;i<50;i++){
			readADC[i]=returnvalues->adc[i];
			if(i<49) readFDEV[i]=returnvalues->fdev[i];
			if(i<48) readSDEV[i]=returnvalues->sdev[i];
		}
		printf("\n");
		

		char buffer[32];

    		snprintf(buffer, sizeof(buffer), "Frequency: %d",returnvalues->freqint);
    		gtk_label_set_text(GTK_LABEL(freqValues), buffer);

		char msg[32];
    		snprintf(msg, sizeof(msg), "%s",returnvalues->message);
    		gtk_label_set_text(GTK_LABEL(msgValues), msg);

		char buffer1[32];
		char buffer2[32];
		char buffer3[32];
		int j=0;

			
    			snprintf(buffer1, sizeof(buffer1), "adcValues: %d %d %d %d %d %d",readADC[0],readADC[1],readADC[2],readADC[3],readADC[4],readADC[5]);
    			gtk_label_set_text(GTK_LABEL(adcValues), buffer1);
		//	if(j<49) {

    				snprintf(buffer2, sizeof(buffer2), "fdevValues: %d",readFDEV[j]);
    				gtk_label_set_text(GTK_LABEL(fdevValues), buffer2);
		//	}	
//			if(j<48) {

    				snprintf(buffer3, sizeof(buffer3), "sdevValues: %d",readSDEV[j]);
    				gtk_label_set_text(GTK_LABEL(sdevValues), buffer3);
//			}

//		}


	
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
    	GtkWidget *button;
    	GThread   *thread;
    	GError    *error = NULL;


    	if( ! g_thread_supported() )
        	g_thread_init( NULL );

	gtk_init(&argc, &argv);



	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW (window) , "PICOSCOPE");
	gtk_container_set_border_width(GTK_CONTAINER(window),300);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	icon=create_pixbuf("analog-icon-17.jpg");
	gtk_window_set_icon(GTK_WINDOW(window),icon);
		
	grid = gtk_grid_new();
	gtk_container_add(GTK_CONTAINER(window), grid);


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

       	msgValues = gtk_label_new(".");
       	gtk_grid_attach(GTK_GRID(grid), msgValues, 0, 41, 5, 5);

	button = gtk_button_new_with_label ("Quit");
        g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_widget_destroy), window);
       	gtk_grid_attach (GTK_GRID (grid), button, 50,0, 1, 1);
	
	gtk_widget_show_all(window);
	g_object_unref(icon);
	gtk_main();

    return 0;
}

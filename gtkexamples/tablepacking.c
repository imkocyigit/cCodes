#include <cairo.h>
#include <gtk/gtk.h>
int sine[]={25,28,31,34,37,40,42,44,
46,48,49,50,50,50,50,49,
48,46,44,42,40,37,34,31,
28,25,22,19,16,13,10,8,
6,4,2,1,0,0,0,0,
1,2,4,6,8,10,13,16,
19,22,25};
static void do_drawing(cairo_t *);

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data) {      
  do_drawing(cr);

  return FALSE;
}

static void do_drawing(cairo_t *cr) {
	cairo_set_source_rgb(cr, 0, 0, 0);
	//cairo_set_source_rgb (cr, 0, 0, 0);
	/*cairo_move_to (cr, 0, 0);
	cairo_line_to (cr, 400, 400);
	cairo_move_to (cr, 400, 0);
	cairo_line_to (cr, 0, 400);i*/
	
	cairo_move_to (cr, 0, 0);
	int i=0;
	for(i=0;i<50;i++) {
		cairo_move_to(cr,i*8,sine[i]*2);
		cairo_line_to(cr,(i+1)*8,sine[i+1]*2);
	}





	cairo_set_line_width (cr, 2);
	cairo_stroke (cr);

	cairo_rectangle (cr, 0, 0, 400, 100);
	cairo_set_source_rgba (cr, 1, 0, 0, 0.5);
	cairo_fill (cr);


  	/*cairo_set_font_size(cr, 10.0);
	cairo_set_line_width(cr,8);
  	cairo_move_to(cr,0, 80);
  	cairo_show_text(cr, "Disziplin ist Macht.");*/    
}
/* Our callback.
 * The data passed to this function is printed to stdout */


static void callback( GtkWidget *widget,
                      gpointer   data )
{
    g_print ("Hello again - %s was pressed\n", (char *) data);
}

/* This callback quits the program */
static gboolean delete_event( GtkWidget *widget,
                              GdkEvent  *event,
                              gpointer   data )
{
    gtk_main_quit ();
    return FALSE;
}

int main( int   argc,
          char *argv[] )
{
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *table;
    GtkWidget *darea;

    gtk_init (&argc, &argv);

    /* Create a new window */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    /* Set the window title */
    gtk_window_set_title (GTK_WINDOW (window), "Table");

    /* Set a handler for delete_event that immediately
     * exits GTK. */
    g_signal_connect (window, "delete-event",
                      G_CALLBACK (delete_event), NULL);

    /* Sets the border width of the window. */
    gtk_container_set_border_width (GTK_CONTAINER (window), 250);

    /* Create a 2x2 table */
    table = gtk_table_new (3, 3, TRUE);

    /* Put the table in the main window */
    gtk_container_add (GTK_CONTAINER (window), table);

    /* Create first button */
    button = gtk_button_new_with_label ("button 1");

    /* When the button is clicked, we call the "callback" function
     * with a pointer to "button 1" as its argument */
    g_signal_connect (button, "clicked",
	              G_CALLBACK (callback), (gpointer) "button 1");


    /* Insert button 1 into the upper left quadrant of the table */
    gtk_table_attach_defaults (GTK_TABLE (table), button, 0, 1, 0, 1);

    gtk_widget_show (button);
    /* create drawing area*/

	darea=gtk_drawing_area_new();
	g_signal_connect(G_OBJECT(darea),"draw",G_CALLBACK(on_draw_event),NULL);
	gtk_table_attach_defaults(GTK_TABLE(table),darea,0,6,2,6);
	gtk_widget_show (darea);











    /* Create second button */

    button = gtk_button_new_with_label ("button 2");

    /* When the button is clicked, we call the "callback" function
     * with a pointer to "button 2" as its argument */
    g_signal_connect (button, "clicked",
                      G_CALLBACK (callback), (gpointer) "button 2");
    /* Insert button 2 into the upper right quadrant of the table */
    gtk_table_attach_defaults (GTK_TABLE (table), button, 1, 2, 0, 1);

    gtk_widget_show (button);

    /* Create "Quit" button */
    button = gtk_button_new_with_label ("Quit");

    /* When the button is clicked, we call the "delete-event" function
     * and the program exits */
    g_signal_connect (button, "clicked",
                      G_CALLBACK (delete_event), NULL);

    /* Insert the quit button into the both 
     * lower quadrants of the table */
    gtk_table_attach_defaults (GTK_TABLE (table), button, 0, 2, 1, 2);

    gtk_widget_show (button);

    gtk_widget_show (table);
    gtk_widget_show (window);

    gtk_main ();

    return 0;
}

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


int main(int argc, char *argv[]) {
  	GtkWidget *window;
  	GtkWidget *darea;

  	gtk_init(&argc, &argv);

  	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  	darea = gtk_drawing_area_new();
  	gtk_container_add(GTK_CONTAINER(window), darea);

  	g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(on_draw_event), NULL); 
  	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  	gtk_window_set_default_size(GTK_WINDOW(window), 400, 400); 
  	gtk_window_set_title(GTK_WINDOW(window), "GTK window");

  	gtk_widget_show_all(window);

  	gtk_main();

  return 0;
}

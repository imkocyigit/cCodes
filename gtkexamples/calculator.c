#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>

static GtkWidget *number1;
static GtkWidget *number2;
static GtkWidget *result;

static void add(GtkWidget *widget, gpointer data) {
       	int num1 = atoi((char *)gtk_entry_get_text(GTK_ENTRY(number1)));
       	int num2 = atoi((char *)gtk_entry_get_text(GTK_ENTRY(number2)));
       
	char buffer[32];
	snprintf(buffer, sizeof(buffer), "= %d", num1 + num2);

	gtk_label_set_text(GTK_LABEL(result), buffer);
}

/*void sub(GtkWidget *widget,gpointer data) {
       	int num1 = atoi((char *)gtk_entry_get_text(GTK_ENTRY(number1)));
       	int num2 = atoi((char *)gtk_entry_get_text(GTK_ENTRY(number2)));
       
	char buffer[32];
	snprintf(buffer, sizeof(buffer), "result: %d", num1 - num2);

	gtk_label_set_text(GTK_LABEL(result), buffer);
}

void mul(GtkWidget *widget, gpointer data) {
       	int num1 = atoi((char *)gtk_entry_get_text(GTK_ENTRY(number1)));
       	int num2 = atoi((char *)gtk_entry_get_text(GTK_ENTRY(number2)));
       
	char buffer[32];
	snprintf(buffer, sizeof(buffer), "result: %d", num1 * num2);

	gtk_label_set_text(GTK_LABEL(result), buffer);
}

void divid(GtkWidget *widget, gpointer data) {
       	int num1 = atoi((char *)gtk_entry_get_text(GTK_ENTRY(number1)));
       	int num2 = atoi((char *)gtk_entry_get_text(GTK_ENTRY(number2)));
       
	char buffer[32];
	snprintf(buffer, sizeof(buffer), "= %d", num1 / num2);

	gtk_label_set_text(GTK_LABEL(result), buffer);
}*/





static void activate(GtkApplication *app, gpointer data) {

	GtkWidget *window;
	GtkWidget *grid;
	GtkWidget *button;

	GtkWidget *add;
	GtkWidget *sub;
	GtkWidget *mul;
	GtkWidget *divid;
	//creating new window
	
	window=gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW (window) , "CALCULAS");
	gtk_container_set_border_width(GTK_CONTAINER (window), 300 );

	//container holds the button
	grid = gtk_grid_new();
	gtk_container_add(GTK_CONTAINER (window), grid);

        number1 = gtk_entry_new();
        gtk_grid_attach(GTK_GRID(grid), number1, 0, 0, 1, 1);

        number2 = gtk_entry_new();
        gtk_grid_attach(GTK_GRID(grid), number2, 1, 0, 1, 1);


	add = gtk_button_new_with_label("+");
	g_signal_connect(add,"clicked",G_CALLBACK(add),NULL);
	gtk_grid_attach(GTK_GRID(grid), add, 3, 0, 1, 1);
	
	sub= gtk_button_new_with_label("-");
	g_signal_connect(sub,"clicked",G_CALLBACK(sub),NULL);
	gtk_grid_attach(GTK_GRID(grid), sub, 3, 1, 1, 1);

	mul = gtk_button_new_with_label("x");
	g_signal_connect(mul,"clicked",G_CALLBACK(mul),NULL);
	gtk_grid_attach(GTK_GRID(grid), mul, 3, 2, 1, 1);

	divid = gtk_button_new_with_label("/");
	g_signal_connect(divid,"clicked",G_CALLBACK(divid),NULL);
	gtk_grid_attach(GTK_GRID(grid), divid, 3, 3, 1, 1);


	button = gtk_button_new_with_label ("Quit");
	g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_widget_destroy), window);
	gtk_grid_attach (GTK_GRID (grid), button, 4, 4, 1, 1);

	result = gtk_label_new("=");
        gtk_grid_attach(GTK_GRID(grid), result, 4, 0, 3, 3);

        gtk_widget_show_all(window);

}

int main (int argc,char **argv) {

	GtkApplication *app;
	int status;

	app=gtk_application_new ("org.gtk.kalkulas", G_APPLICATION_FLAGS_NONE);
	g_signal_connect (app,"activate",G_CALLBACK(activate),NULL);
	status=g_application_run (G_APPLICATION (app) , argc ,argv);
	g_object_unref (app);

	return status;
}












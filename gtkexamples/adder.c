#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

static GtkWidget *number1;
static GtkWidget *number2;
static GtkWidget *result;

void do_add(GtkWidget *calculate, gpointer data) {
    int num1 = atoi((char *)gtk_entry_get_text(GTK_ENTRY(number1)));
    int num2 = atoi((char *)gtk_entry_get_text(GTK_ENTRY(number2)));

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "result: %d", num1 + num2);

    gtk_label_set_text(GTK_LABEL(result), buffer);
}


void do_sub(GtkWidget *calculate, gpointer data) {
    int num1 = atoi((char *)gtk_entry_get_text(GTK_ENTRY(number1)));
    int num2 = atoi((char *)gtk_entry_get_text(GTK_ENTRY(number2)));

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "result: %d", num1 - num2);

    gtk_label_set_text(GTK_LABEL(result), buffer);
}


void do_mul(GtkWidget *calculate, gpointer data) {
    int num1 = atoi((char *)gtk_entry_get_text(GTK_ENTRY(number1)));
    int num2 = atoi((char *)gtk_entry_get_text(GTK_ENTRY(number2)));

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "result: %d", num1 * num2);

    gtk_label_set_text(GTK_LABEL(result), buffer);
}


void do_divid(GtkWidget *calculate, gpointer data) {
    int num1 = atoi((char *)gtk_entry_get_text(GTK_ENTRY(number1)));
    int num2 = atoi((char *)gtk_entry_get_text(GTK_ENTRY(number2)));

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "result: %d", num1 / num2);

    gtk_label_set_text(GTK_LABEL(result), buffer);
}

int main(int argc, char **argv) {
       	GtkWidget *window, *grid, *add,*sub,*mul,*divid,*button;
	gtk_init(&argc, &argv);



	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW (window) , "CALCULAS");
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	grid = gtk_grid_new();
	gtk_container_add(GTK_CONTAINER(window), grid);

	number1 = gtk_entry_new();
	gtk_grid_attach(GTK_GRID(grid), number1, 0, 0, 1, 1);

	number2 = gtk_entry_new();
	gtk_grid_attach(GTK_GRID(grid), number2, 1, 0, 1, 1);

	add = gtk_button_new_with_label("+");
	g_signal_connect(add, "clicked", G_CALLBACK(do_add), NULL);
	gtk_grid_attach(GTK_GRID(grid), add, 2, 0, 1, 1);


	sub = gtk_button_new_with_label("-");
	g_signal_connect(sub, "clicked", G_CALLBACK(do_sub), NULL);
	gtk_grid_attach(GTK_GRID(grid),sub , 2, 1, 1, 1);


	mul = gtk_button_new_with_label("x");
	g_signal_connect(mul, "clicked", G_CALLBACK(do_mul), NULL);
	gtk_grid_attach(GTK_GRID(grid), mul, 2, 2, 1, 1);


	divid = gtk_button_new_with_label("/");
	g_signal_connect(divid, "clicked", G_CALLBACK(do_divid), NULL);
	gtk_grid_attach(GTK_GRID(grid), divid, 2, 3, 1, 1);

       	result = gtk_label_new("result:");
       	gtk_grid_attach(GTK_GRID(grid), result, 3, 0, 1, 1);


	button = gtk_button_new_with_label ("Quit");
        g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_widget_destroy), window);
        gtk_grid_attach (GTK_GRID (grid), button, 4, 4, 1, 1);
	
	gtk_widget_show_all(window);
	gtk_main();

    return 0;
}

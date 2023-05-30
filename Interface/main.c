#include <gtk/gtk.h>
#include "Network/client.h"

int socketClient;
GtkWidget *top_comment1;
Infos* utilisateur;

char* get_movie_watched()
{
    socketClient = connect_to_server("127.0.0.1",6969);
    write_server_int(socketClient,2);
    write_infos_to_server(socketClient,utilisateur);
    char* buf = malloc(2048);
    read_server(socketClient,buf);
    return buf;
}



// Callback function for the "destroy" signal of the window
void on_window_destroy(GtkWidget *widget, gpointer user_data) 
{
    gtk_main_quit();
}

// Callback function for the "clicked" signal of the login button
void on_login_button_clicked(GtkWidget *widget, gpointer user_data) 
{
    GtkBuilder *builder = GTK_BUILDER(user_data);
    GtkWidget *login_dialog = GTK_WIDGET(gtk_builder_get_object(builder, "login_dialog"));
    gtk_dialog_run(GTK_DIALOG(login_dialog));
    gtk_widget_hide(login_dialog);
}

// Callback function for the "clicked" signal of the create button
void on_create_button_clicked(GtkWidget *widget, gpointer user_data) 
{
    GtkBuilder *builder = GTK_BUILDER(user_data);
    GtkWidget *create_account_dialog = GTK_WIDGET(gtk_builder_get_object(builder, "create_account_dialog"));
    gtk_dialog_run(GTK_DIALOG(create_account_dialog));
    gtk_widget_hide(create_account_dialog);
}

// Callback function for the "clicked" signal of the login button in login dialog
void on_login_button_popup_clicked(GtkWidget *widget, gpointer user_data) 
{
    socketClient = connect_to_server("127.0.0.1",6969);
    GtkBuilder *builder = GTK_BUILDER(user_data);
    GtkEntry *username_entry = GTK_ENTRY(gtk_builder_get_object(builder, "username_entry"));
    GtkEntry *password_entry = GTK_ENTRY(gtk_builder_get_object(builder, "password_entry"));
    const gchar *username_text = gtk_entry_get_text(username_entry);
    const gchar *password_text = gtk_entry_get_text(password_entry);
    char* films = malloc(5128);
    write_server_int(socketClient,0);
    int n;
    if((n = sign_in(socketClient,username_text,password_text,"")))
    {
        printf("Incorrect username or password\n");
    }else
    {
        utilisateur = malloc(sizeof(Infos));
        strcpy(utilisateur->username,username_text);
        strcpy(utilisateur->password,password_text);
        strcpy(utilisateur->email,"");
        char* res = get_movie_watched();
        char* token = strtok(res,"|");
        sprintf(films,"%s",token);
        token = strtok(NULL,"|");
        int n = 0;
        while(token!= NULL && n < 10)
        {
            sprintf(films,"%s%s\n",films,token);
            token = strtok(NULL,"|");
            n++;
        }

        printf("You are Online!\n");
    };
    gtk_label_set_text(GTK_LABEL(top_comment1),(gchar*) films);
    g_object_set_data(G_OBJECT(widget), "username_text", g_strdup(username_text));
    g_object_set_data(G_OBJECT(widget), "password_text", g_strdup(password_text));
    close(socketClient);
    free(films);
}
// Callback function for the "clicked" signal of the create button popup
void on_create_button_popup_clicked(GtkWidget *widget, gpointer user_data) 
{
    GtkBuilder *builder = GTK_BUILDER(user_data);
    GtkWidget *login_dialog = GTK_WIDGET(gtk_builder_get_object(builder, "login_dialog"));
    GtkWidget *create_account_dialog = GTK_WIDGET(gtk_builder_get_object(builder, "create_account_dialog"));
    gtk_widget_hide(login_dialog);
    gtk_dialog_run(GTK_DIALOG(create_account_dialog));
    gtk_widget_hide(create_account_dialog);
}

// Callback function for the "clicked" signal of the search button
void on_search_button_clicked(GtkWidget *widget, gpointer user_data) 
{
    GtkBuilder *builder = GTK_BUILDER(user_data);
    GtkEntry *search_bar = GTK_ENTRY(gtk_builder_get_object(builder, "search_bar"));
    const char* search_text = gtk_entry_get_text(search_bar);
    g_object_set_data(G_OBJECT(widget), "search_text", g_strdup(search_text));
}

// Callback function for the "clicked" signal of the cancel button in create_account dialog
void on_cancel_button_clicked(GtkWidget *widget, gpointer user_data) 
{
    GtkWidget *create_account_dialog = GTK_WIDGET(user_data);
    gtk_widget_hide(create_account_dialog);
}

// Callback function for the "clicked" signal of the confirm button in create_account dialog
void on_confirm_button_clicked(GtkWidget *widget, gpointer user_data) 
{
    socketClient = connect_to_server("127.0.0.1",6969);
    GtkBuilder *builder = GTK_BUILDER(user_data);
    GtkEntry *username_entry2 = GTK_ENTRY(gtk_builder_get_object(builder, "username_entry2"));
    GtkEntry *password_entry2 = GTK_ENTRY(gtk_builder_get_object(builder, "password_entry2"));
    GtkEntry *confirm_entry = GTK_ENTRY(gtk_builder_get_object(builder, "confirm_entry"));

    write_server_int(socketClient,1);

    const gchar *username_text2 = gtk_entry_get_text(username_entry2);
    const gchar *password_text2 = gtk_entry_get_text(password_entry2);
    const gchar *confirm_text = gtk_entry_get_text(confirm_entry);

    // Check if the password and confirm text are the same
    if (g_strcmp0(password_text2, confirm_text) == 0) 
    {
        g_object_set_data(G_OBJECT(widget), "username_text2", g_strdup(username_text2));
        g_object_set_data(G_OBJECT(widget), "password_text2", g_strdup(password_text2));
        char* email = malloc(2048);
        *email = 0;
        sign_up(socketClient,username_text2,password_text2,email);
        printf("You are SIGN UP, Now go LOGIN \n");
    } 
    else 
    {
        // Show error message dialog
        GtkWidget *error_message = GTK_WIDGET(gtk_builder_get_object(builder, "error_message"));
        gtk_dialog_run(GTK_DIALOG(error_message));
        gtk_widget_hide(error_message);
        return;
    }
    close(socketClient);
}

int main(int argc, char *argv[]) 
{

    

    // Initialize GTK
    gtk_init(&argc, &argv);

    // Load the Glade file
    GtkBuilder *builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "watchif.glade", NULL);

    // Get the main window widget from the Glade file
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));

    // Connect the "destroy" signal to the on_window_destroy() callback
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), NULL);

    // Get the buttons widgets from the Glade file
    GtkWidget *login_button = GTK_WIDGET(gtk_builder_get_object(builder, "login_button"));
    GtkWidget *create_button = GTK_WIDGET(gtk_builder_get_object(builder, "create_button"));
    GtkWidget *login_button_popup = GTK_WIDGET(gtk_builder_get_object(builder, "login_button_popup"));
    GtkWidget *create_button_popup = GTK_WIDGET(gtk_builder_get_object(builder, "create_button_popup"));
    GtkWidget *search_button = GTK_WIDGET(gtk_builder_get_object(builder, "search_button"));
    GtkWidget *cancel_button = GTK_WIDGET(gtk_builder_get_object(builder, "cancel_button"));
    GtkWidget *confirm_button = GTK_WIDGET(gtk_builder_get_object(builder, "confirm_button"));
    top_comment1 = GTK_WIDGET(gtk_builder_get_object(builder, "top_comment1"));

    // Connect the "clicked" signal to the buttons callbacks
    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_button_clicked), builder);
    g_signal_connect(create_button, "clicked", G_CALLBACK(on_create_button_clicked), builder);
    g_signal_connect(login_button_popup, "clicked", G_CALLBACK(on_login_button_popup_clicked), builder);
    g_signal_connect(create_button_popup, "clicked", G_CALLBACK(on_create_button_popup_clicked), builder);
    g_signal_connect(search_button, "clicked", G_CALLBACK(on_search_button_clicked), builder);
    g_signal_connect(cancel_button, "clicked", G_CALLBACK(on_cancel_button_clicked), builder);
    g_signal_connect(confirm_button, "clicked", G_CALLBACK(on_confirm_button_clicked), builder);

    // Show the window and enter the GTK main loop
    gtk_widget_show_all(window);
    gtk_main();

    // Retrieve the search_text from the search button widget
    gchar* search_text = g_object_get_data(G_OBJECT(search_button), "search_text");
    if (search_text != NULL) 
    {
        // TODO: remove gprint
        g_print("You searched for : '%s'\n", search_text);
        g_free((gpointer)search_text);
    }

    // Retrieve the login account information
    const gchar *username_text = g_object_get_data(G_OBJECT(confirm_button), "username_text");
    const gchar *password_text = g_object_get_data(G_OBJECT(confirm_button), "password_text");
    if (username_text != NULL && password_text != NULL) 
    {
        // TODO: remove g_print
        g_print("Login Information: Username - %s, Password - %s\n", username_text, password_text);
        g_free((gpointer)username_text);
        g_free((gpointer)password_text);
    }

    // Retrieve the create accgcc ui.c recommender.c kmeans.c matrix_normalization.c pearsons.c predictions.c sorting.c utility_matrix.c -g -lm -lsqlite3ount information
    const gchar *username_text2 = g_object_get_data(G_OBJECT(confirm_button), "username_text2");
    const gchar *password_text2 = g_object_get_data(G_OBJECT(confirm_button), "password_text2");
    if (username_text != NULL && password_text != NULL) 
    {
        // TODO: remove g_print 
        g_print("Create account Information: Username - %s, Password - %s\n", username_text2, password_text2);
        g_free((gpointer)username_text2);
        g_free((gpointer)password_text2);
    }

    return 0;
}

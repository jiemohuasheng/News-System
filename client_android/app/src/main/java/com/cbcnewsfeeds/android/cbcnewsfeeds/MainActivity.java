package com.cbcnewsfeeds.android.cbcnewsfeeds;

import android.app.Presentation;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.drawable.Drawable;
import android.support.v4.app.LoaderManager;
import android.support.v4.content.AsyncTaskLoader;
import android.support.v4.content.Loader;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.preference.PreferenceManager;
import android.support.v7.widget.DividerItemDecoration;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.text.TextUtils;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;
import java.net.URL;
import java.io.InputStream;
import java.net.UnknownHostException;
import java.sql.Struct;
import java.util.ArrayList;
import java.util.List;

import com.cbcnewsfeeds.android.cbcnewsfeeds.Adapter.SlidingMenuAdapter;
import com.cbcnewsfeeds.android.cbcnewsfeeds.Model.ItemSlideMenu;
import com.jcraft.jsch.Channel;
import com.jcraft.jsch.ChannelExec;
import com.jcraft.jsch.JSch;
import com.jcraft.jsch.Session;

import org.simpleframework.xml.Serializer;
import org.simpleframework.xml.core.Persister;

//import static android.R.attr.data;


public class MainActivity extends AppCompatActivity
        implements LoaderManager.LoaderCallbacks<String>,NewsAdapter.NewsItemClickListener,SharedPreferences.OnSharedPreferenceChangeListener{
    private RecyclerView recyclerView;
    private static final int CONNECTION_CREATION_LOADER = 20,SEND_REQUEST_LOADER = 22;
    private Toast mToast;
    private static Socket sock;
    private List<ItemSlideMenu> listSliding;
    private SlidingMenuAdapter adapter;
    private ListView listViewSliding;
    private DrawerLayout drawerLayout;
    private ActionBarDrawerToggle actionBarDrawerToggle;
    public static List<NewsStructure> newsStructureList;
    String serverName;
    PrintWriter pwrite;
    BufferedReader receiveRead;
    OutputStream ostream;

    int port;
    String requestMessage;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        setupSharedPreference();

        //****************************
        //Init component
        listViewSliding = (ListView) findViewById(R.id.lv_sliding_menu);
        drawerLayout = (DrawerLayout) findViewById(R.id.drawer_layout);
        listSliding = new ArrayList<>();
        //Add item for sliding list
        listSliding.add(new ItemSlideMenu(R.drawable.topstories, "Top Stories"));
        listSliding.add(new ItemSlideMenu(R.drawable.canada, "Canada"));
        listSliding.add(new ItemSlideMenu(R.drawable.sports, "Sports"));
        listSliding.add(new ItemSlideMenu(R.drawable.politics, "Politics"));
        listSliding.add(new ItemSlideMenu(R.drawable.business, "Business"));
        listSliding.add(new ItemSlideMenu(R.drawable.tech, "Tech"));
        listSliding.add(new ItemSlideMenu(R.drawable.windsor, "Windsor"));
        listSliding.add(new ItemSlideMenu(R.drawable.nba, "NBA"));
        listSliding.add(new ItemSlideMenu(R.drawable.soccer, "Soccer"));
        adapter = new SlidingMenuAdapter(this, listSliding);
        listViewSliding.setAdapter(adapter);
        //Display icon to open/ close sliding list
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        //Set title
        //setTitle(listSliding.get(0).getTitle());
        //item selected
        listViewSliding.setItemChecked(0, true);
        //Close menu
        drawerLayout.closeDrawer(listViewSliding);
        listViewSliding.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                //Set title
                //setTitle(listSliding.get(position).getTitle());
                //item selected
                listViewSliding.setItemChecked(position, true);

                //Close menu
                drawerLayout.closeDrawer(listViewSliding);

                if (mToast != null) {
                    mToast.cancel();
                }
                requestMessage = listSliding.get(position).getTitle();
                String toastMessage = "Item #" +listSliding.get(position).getTitle() + " clicked.";
                mToast = Toast.makeText(MainActivity.this, toastMessage, Toast.LENGTH_LONG);
                mToast.show();
                Bundle queryBundle = new Bundle();
                // COMPLETED (20) Use putString with SEARCH_QUERY_URL_EXTRA as the key and the String value of the URL as the value
                queryBundle.putString("keyOfBundle","ValueofBundle");

                Log.d("TagForTest","TestStringgg");
                LoaderManager loaderManager = getSupportLoaderManager();
                Loader<String> githubSearchLoader = loaderManager.getLoader(SEND_REQUEST_LOADER);
                // COMPLETED (23) If the Loader was null, initialize it. Else, restart it.
                if (githubSearchLoader == null) {
                    loaderManager.initLoader(SEND_REQUEST_LOADER,queryBundle, MainActivity.this);
                } else {
                    loaderManager.restartLoader(SEND_REQUEST_LOADER, queryBundle, MainActivity.this);
                }
            }
        });
        actionBarDrawerToggle = new ActionBarDrawerToggle(this, drawerLayout, R.string.drawer_opened, R.string.drawer_closed){

            @Override
            public void onDrawerOpened(View drawerView) {
                super.onDrawerOpened(drawerView);
                invalidateOptionsMenu();
            }

            @Override
            public void onDrawerClosed(View drawerView) {
                super.onDrawerClosed(drawerView);
                invalidateOptionsMenu();
            }
        };

        drawerLayout.setDrawerListener(actionBarDrawerToggle);
        //****************************


        Bundle queryBundle = new Bundle();
        // COMPLETED (20) Use putString with SEARCH_QUERY_URL_EXTRA as the key and the String value of the URL as the value
        queryBundle.putString("keyOfBundle","ValueofBundle");

        Log.d("TagForTest","TestStringgg");
        LoaderManager loaderManager = getSupportLoaderManager();
        // COMPLETED (22) Get our Loader by calling getLoader and passing the ID we specified
        Loader<String> githubSearchLoader = loaderManager.getLoader(CONNECTION_CREATION_LOADER);
        // COMPLETED (23) If the Loader was null, initialize it. Else, restart it.
        if (githubSearchLoader == null) {
            loaderManager.initLoader(CONNECTION_CREATION_LOADER,queryBundle, this);
        } else {
            loaderManager.restartLoader(CONNECTION_CREATION_LOADER, queryBundle, this);
        }


    }

    private void setupSharedPreference() {
        SharedPreferences sharedPreferences = PreferenceManager.getDefaultSharedPreferences(this);
        serverName = sharedPreferences.getString(getString(R.string.pref_host_key_name),
                getString(R.string.pref_host_label_alpha));
        port = Integer.parseInt(sharedPreferences.getString(getString(R.string.pref_port_no_key_name),
                getString(R.string.pref_port_no_default)));
        Log.d("TTTTT",sharedPreferences.getString(getString(R.string.pref_host_key_name),
                getString(R.string.pref_host_charlie_value)));


    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.main_menu, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();
        if (id == R.id.settings) {
            Intent startSettingsActivity = new Intent(this, SettingsActivity.class);
            startActivity(startSettingsActivity);
            return true;
        }
        if(actionBarDrawerToggle.onOptionsItemSelected(item)) {
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

    @Override
    protected void onPostCreate(Bundle savedInstanceState) {
        super.onPostCreate(savedInstanceState);
        actionBarDrawerToggle.syncState();
    }
    @Override
    public Loader<String> onCreateLoader(final int id, Bundle args) {
        if (id == CONNECTION_CREATION_LOADER){
            return new AsyncTaskLoader<String>(this) {

                // COMPLETED (5) Override onStartLoading

                @Override
                protected void onStartLoading() {
                    Log.d("onstartloading called", "yesss");
                    // COMPLETED (6) If args is null, return.
                /* If no arguments were passed, we don't have a query to perform. Simply return. */

                    // COMPLETED (8) Force a load
                    forceLoad();
                }

                // COMPLETED (9) Override loadInBackground
                @Override
                public String loadInBackground() {

                    //serverName = getString(R.string.server_name);
                    //port = Integer.parseInt(getString(R.string.port_name));
                    //Socket sock = null;


                    //soc.setSoTimeout(SO_TIMEOUT); // if you like
// connect (with timeout)


                    String error = "false";
                    try {

                        Log.d("hhhggg1", "Just connected to ");
                        sock = new Socket();
                        sock.connect(new InetSocketAddress(serverName, port), 3000);
                        //sock = new Socket(serverName, port);
                        Log.d("hhhggg2", "Just connected to ");

                        Log.d("hhhggg", "Just connected to " + sock.getRemoteSocketAddress());
                         ostream = sock.getOutputStream();
                        pwrite = new PrintWriter(ostream, true);
                        InputStream istream = sock.getInputStream();
                        receiveRead = new BufferedReader(new InputStreamReader(istream));
                        String receiveMessage, sendMessage;
                        sendMessage = "Testttttt";//keyRead.readLine();  // keyboard reading
                        pwrite.println(sendMessage);       // sending to server
                        pwrite.flush();                    // flush the data
                        List<String> tempStringList = new ArrayList<String>();
                        ;
                        String unmarshalledString = "";
                        while (!(receiveMessage = receiveRead.readLine()).contains("Bye")) //receive from server
                        {
                            Log.d("ReceivedFromServer", receiveMessage); // displaying at DOS prompt
                            unmarshalledString = receiveMessage;
                            tempStringList.add(receiveMessage);
                        }
//
                        Log.d("unmarshaldata", unmarshalledString);
                        Serializer serializer = new Persister();
                        ResponseData responseData = serializer.read(ResponseData.class, unmarshalledString);
                        Log.d("umarshal", "success");
                        Log.d("unmarshal", responseData.getItemGroup().get(0).getItem().get(1).getAuthor());
                        NewsStructure newsStructure = new NewsStructure();
                        newsStructureList = new ArrayList<NewsStructure>();
                        for (ItemGroup itemGroup : responseData.getItemGroup()) {
                            for (Item item : itemGroup.getItem()) {
                                newsStructure = new NewsStructure();
                                newsStructure.setPubTime(item.getPubTime());
                                newsStructure.setAuthor(item.getAuthor());
                                newsStructure.setCategory(itemGroup.getCategory());
                                newsStructure.setUrl(item.getUrl());
                                newsStructure.setNewsHeading(item.getHeading());
                                newsStructure.setNewsDescription(item.getDescription());
                                newsStructure.setImage(LoadImageFromWebOperations(item.getImg()));
                                newsStructureList.add(newsStructure);
                            }
                        }
                        Log.d("newsstructurelist", newsStructureList.get(0).getNewsHeading());


                    } catch (Exception e1) {
                        Log.d("Errr", "sorryErrorOccured");
                        error = "true";
                        e1.printStackTrace();
                    }
                    return error;

                }
            };
    }
        else{
            return new AsyncTaskLoader<String>(this) {
                @Override
                protected void onStartLoading() {
                    Log.d("onstartloading called", "yesss");
                    // COMPLETED (6) If args is null, return.
                /* If no arguments were passed, we don't have a query to perform. Simply return. */

                    // COMPLETED (8) Force a load
                    forceLoad();
                }
                @Override
                public String loadInBackground() {
                    try {
                        String receiveMessage, sendMessage;
                        //sendMessage = "Testttttt";//keyRead.readLine();  // keyboard reading
                        pwrite = new PrintWriter(ostream, true);

                        pwrite.println(requestMessage);       // sending to server
                        pwrite.flush();                    // flush the data
                        List<String> tempStringList = new ArrayList<String>();
                        ;
                        String unmarshalledString = "";
                        while (!(receiveMessage = receiveRead.readLine()).contains("Bye")) //receive from server
                        {
                            Log.d("ReceivedFromServer", receiveMessage); // displaying at DOS prompt
                            unmarshalledString = receiveMessage;
                            tempStringList.add(receiveMessage);
                        }
//
                        Log.d("unmarshaldata", unmarshalledString);
                        Serializer serializer = new Persister();
                        ResponseData responseData = serializer.read(ResponseData.class, unmarshalledString);
                        Log.d("umarshal", "success");
                        Log.d("unmarshal", responseData.getItemGroup().get(0).getItem().get(1).getAuthor());
                        NewsStructure newsStructure = new NewsStructure();
                        newsStructureList = new ArrayList<NewsStructure>();
                        for (ItemGroup itemGroup : responseData.getItemGroup()) {
                            for (Item item : itemGroup.getItem()) {
                                newsStructure = new NewsStructure();
                                newsStructure.setPubTime(item.getPubTime());
                                newsStructure.setAuthor(item.getAuthor());
                                newsStructure.setCategory(itemGroup.getCategory());
                                newsStructure.setUrl(item.getUrl());
                                newsStructure.setNewsHeading(item.getHeading());
                                newsStructure.setNewsDescription(item.getDescription());
                                newsStructure.setImage(LoadImageFromWebOperations(item.getImg()));
                                newsStructureList.add(newsStructure);
                            }
                        }
                        Log.d("newsstructurelist", newsStructureList.get(0).getNewsHeading());
                        sock.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                    return null;
                }
            };
        }
    }

    @Override
    public void onLoadFinished(Loader<String> loader, String error) {
        if(loader.getId()==CONNECTION_CREATION_LOADER){
            Log.d("Func","OnloadFinished invoked"+error);
            if(error.equalsIgnoreCase("false")) {
                showRecyclerView(true);
                NewsAdapter newsAdapter;
                newsAdapter = new NewsAdapter(this);

                LinearLayoutManager layoutManager
                        = new LinearLayoutManager(this, LinearLayoutManager.VERTICAL, false);
// COMPLETED (41) Set the layoutManager on mRecyclerView
                recyclerView = (RecyclerView) findViewById(R.id.recyclerview_news);
                recyclerView.setLayoutManager(layoutManager);
                recyclerView.setAdapter(newsAdapter);
                DividerItemDecoration dividerItemDecoration = new DividerItemDecoration(recyclerView.getContext(),
                        layoutManager.getOrientation());
                recyclerView.addItemDecoration(dividerItemDecoration);

            }
            else{
                showRecyclerView(false);
            }
        }

    }

    private void showRecyclerView(boolean b) {
        RecyclerView ev = (RecyclerView)findViewById(R.id.recyclerview_news);
        TextView tv = (TextView)findViewById(R.id.error_in_connection);
        if(b == true){
            ev.setVisibility(View.VISIBLE);
            tv.setVisibility(View.INVISIBLE);
        }
        else{
            tv.setText(R.string.connection_error_message);
            ev.setVisibility(View.INVISIBLE);
            tv.setVisibility(View.VISIBLE);
        }

    }

    @Override
    public void onLoaderReset(Loader<String> loader) {

    }
    public static Drawable LoadImageFromWebOperations(String url) {
        try {
            InputStream is = (InputStream) new URL(url).getContent();
            Drawable d = Drawable.createFromStream(is, "src name");
            return d;
        } catch (Exception e) {
            return null;
        }
    }
    //Function to check the click
    @Override
    public void onListItemClick(int clickedItemIndex) {
        
        if (mToast != null) {
            mToast.cancel();
        }
        String toastMessage = "Item #" + clickedItemIndex + " clicked.";
        mToast = Toast.makeText(this, toastMessage, Toast.LENGTH_LONG);

        mToast.show();

    }

    @Override
    public void onSharedPreferenceChanged(SharedPreferences sharedPreferences, String key) {
        //SharedPreferences sharedPreferences = PreferenceManager.getDefaultSharedPreferences(this);
        serverName = sharedPreferences.getString(getString(R.string.pref_host_key_name),
                getString(R.string.pref_host_label_alpha));
        port = Integer.parseInt(sharedPreferences.getString(getString(R.string.pref_port_no_key_name),
                getString(R.string.pref_port_no_default)));
        LoaderManager loaderManager = getSupportLoaderManager();
        // COMPLETED (22) Get our Loader by calling getLoader and passing the ID we specified
        Loader<String> githubSearchLoader = loaderManager.getLoader(CONNECTION_CREATION_LOADER);
        // COMPLETED (23) If the Loader was null, initialize it. Else, restart it.
        Bundle queryBundle = new Bundle();
        // COMPLETED (20) Use putString with SEARCH_QUERY_URL_EXTRA as the key and the String value of the URL as the value
        queryBundle.putString("keyOfBundle","ValueofBundle");
        if (githubSearchLoader == null) {
            loaderManager.initLoader(CONNECTION_CREATION_LOADER,queryBundle, this);
        } else {
            loaderManager.restartLoader(CONNECTION_CREATION_LOADER, queryBundle, this);
        }
    }
}

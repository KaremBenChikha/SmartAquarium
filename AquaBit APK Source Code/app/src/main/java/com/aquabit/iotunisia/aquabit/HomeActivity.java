package com.aquabit.iotunisia.aquabit;

import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Build;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.CardView;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;

public class HomeActivity extends AppCompatActivity implements View.OnClickListener {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_home);

        getSupportActionBar().setElevation(0);

        CardView cardView_1 = findViewById(R.id.temp_card);
        cardView_1.setOnClickListener(this);
        CardView cardView_2 = findViewById(R.id.color_card);
        cardView_2.setOnClickListener(this);
        CardView cardView_3 = findViewById(R.id.feed_card);
        cardView_3.setOnClickListener(this);
        CardView cardView_4 = findViewById(R.id.com_card);
        cardView_4.setOnClickListener(this);
        CardView cardView_5 = findViewById(R.id.stream_card);
        cardView_5.setOnClickListener(this);
        CardView cardView_6 = findViewById(R.id.other_card);
        cardView_6.setOnClickListener(this);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.actions_menu, menu);
        return true;
    }

    // inflating the toolbar layout and handle event clicks
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // The action bar home/up action should open or close the drawer.
        switch (item.getItemId()) {
            case android.R.id.home:
                return true;
            case R.id.actionRefresh:
                Intent intent = (Intent) getIntent();
                finish();
                startActivity(intent);
                return true;
        }

        return super.onOptionsItemSelected(item);
    }

    @Override
    public void onClick(View view) {
        if (view.getId() == R.id.other_card) {
            String body = null;
            try {
                body = getPackageManager().getPackageInfo(getPackageName(), 0).versionName;
                body = "\n\n-----------------------------\nPlease don't remove this information\n Device OS: Android \n Device OS version: " +
                        Build.VERSION.RELEASE + "\n App Version: " + body + "\n Device Brand: " + Build.BRAND +
                        "\n Device Model: " + Build.MODEL + "\n Device Manufacturer: " + Build.MANUFACTURER;
            } catch (PackageManager.NameNotFoundException e) {
            }
            Intent intent = new Intent(Intent.ACTION_SEND);
            intent.setType("message/rfc822");
            intent.putExtra(Intent.EXTRA_EMAIL, new String[]{"contact@androidhive.info"});
            intent.putExtra(Intent.EXTRA_SUBJECT, "Query from AquaBit app");
            intent.putExtra(Intent.EXTRA_TEXT, body);
            startActivity(Intent.createChooser(intent, getString(R.string.choose_email_client)));
        }
        else {
            overridePendingTransition(R.anim.enter, R.anim.exit);
            startActivity(new Intent(HomeActivity.this, NavigateWidgets.class));
        }
    }
}

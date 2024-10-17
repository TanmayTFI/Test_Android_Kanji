package com.example.test_android_kanji;

//android module imports

import static androidx.credentials.GetCredentialRequest.*;

import io.reactivex.Single;
import io.reactivex.disposables.Disposable;
import kotlinx.coroutines.CoroutineScope;
import kotlinx.coroutines.Dispatchers;


import android.content.DialogInterface;
import android.content.Intent;
import android.os.Build;
import android.os.CancellationSignal;
import android.os.Environment;
import android.os.Bundle;
import android.util.Log;
import android.view.WindowManager;
import android.view.KeyEvent;

import android.app.AlertDialog;

//Java Imports
import androidx.annotation.NonNull;
import androidx.credentials.ClearCredentialStateRequest;
import androidx.credentials.Credential;
import androidx.credentials.CredentialManager;
import androidx.credentials.CredentialManagerCallback;
import androidx.credentials.CredentialOption;
import androidx.credentials.CustomCredential;
import androidx.credentials.GetCredentialRequest;
import androidx.credentials.GetCredentialResponse;
import androidx.credentials.GetPublicKeyCredentialOption;
import androidx.credentials.PasswordCredential;
import androidx.credentials.PublicKeyCredential;
import androidx.credentials.exceptions.GetCredentialException;
import androidx.datastore.preferences.core.Preferences;
import androidx.datastore.preferences.core.PreferencesKeys;
import androidx.datastore.rxjava2.RxDataStore;
import androidx.datastore.preferences.rxjava2.RxPreferenceDataStoreBuilder;
import io.reactivex.Completable;
import io.reactivex.schedulers.Schedulers;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.BitSet;
import java.util.UUID;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

//Android gms modules
import com.google.android.gms.auth.api.signin.GoogleSignIn;
import com.google.android.gms.auth.api.signin.GoogleSignInAccount;
import com.google.android.gms.auth.api.signin.GoogleSignInClient;
import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.GoogleApiAvailability;
import com.google.android.gms.common.GooglePlayServicesUtil;
import com.google.android.gms.common.api.ApiException;
import com.google.android.gms.games.AchievementsClient;
import com.google.android.gms.games.EventsClient;
import com.google.android.gms.games.PlayGames;
import com.google.android.gms.games.Player;
import com.google.android.gms.games.PlayersClient;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;


import com.bigfishgames.kanji.KanjiActivity;
import com.bigfishgames.kanji.KanjiGameLib;

import com.example.test_android_kanji.databinding.ActivityMainBinding;
import com.google.android.libraries.identity.googleid.GetGoogleIdOption;
import com.google.android.libraries.identity.googleid.GoogleIdTokenCredential;
import com.google.android.libraries.identity.googleid.GoogleIdTokenParsingException;
import com.google.firebase.FirebaseApp;

import com.google.firebase.analytics.FirebaseAnalytics;
import com.google.firebase.auth.AuthCredential;
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.auth.GoogleAuthProvider;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import io.reactivex.Flowable;


public class MainActivity extends KanjiActivity {

    private static final int REQUEST_CODE = 230815045;
    private ActivityMainBinding binding;
    private static final String TAG = "DarkartaMainActivity";
    private static final String LOG_DOWNLOAD_TAG = "Darkarta Downloader";

    //    private boolean writeExtnlStorage = false;
//    private boolean readExtnlStorage = false;
//    private boolean gamestarted = false;
//    private boolean writeExtnlStorageRequested = false;
//    private boolean readExtnlStorageRequested = false;

    // Variables for Google Play Services part

    // Client used to sign in with Google APIs
    private GoogleSignInClient mGoogleSignInClient;
    // Client variables
    private AchievementsClient mAchievementsClient;
    private BitSet AccomplishmentsCompleted; // Bitset of Acheivement is comeplete in the game.
    private BitSet AccomplishmentsUnlocked; // Bitset of Acheivement that have their unlock sent to google play.

//    private static final int GAME_CAMERA_REQUEST_CODE = 11001;
//    private static final int GAME_EXTRNL_READ_REQUEST_CODE = 11002;
//    private static final int GAME_EXTRNL_WRITE_REQUEST_CODE = 11003;

    private static final int RC_UNUSED = 5001;
    private static final int RC_SIGN_IN = 9001;

    private FirebaseAuth mAuth;
    private DatabaseReference fireDataRef;
    private UserInfo user;

    //Credential Manager
    CredentialManager credentialManager;

    private RxDataStore<Preferences> dataStore;

//    private String client_id = "345938738506-fn7cedlq338jrr3b9cfd4mcbr5us8a1u.apps.googleusercontent.com";
    private File config;


    public static boolean syncHints, unlockGame;
    private native boolean handleKeyEventNative(int keyCode, int action);

    @Override
    public void onCreate(Bundle savedInstanceState)
    {

        FirebaseApp.initializeApp(this);
        super.onCreate(savedInstanceState);
        // Use your app or activity context to instantiate a client instance of


        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        syncHints = false;

        AccomplishmentsCompleted = new BitSet(); // initiate the Bitsets
        AccomplishmentsUnlocked = new BitSet(); // initiate the Bitsets

        System.out.println(AccomplishmentsCompleted);
        System.out.println(AccomplishmentsUnlocked);

        boolean googleFree = true;
        if(BuildConfig.BUILD_VARIANT.equals("amazon"))
        {
            googleFree = true;
            DownloadAssets();
        }
        else
        {
            googleFree = false;
            config = new File(this.getObbDir() + "/config.txt");
            if (config.exists()) {
                String config_txt = readFromFile(config);
                Log.d("ConfigFile", config_txt);
                if (config_txt.equals("noGPG"))// == "noGPG")
                {
                    googleFree = true;
                    Log.d("ConfigFile", "Skipping GPG");
                    DownloadAssets();
                }
                else
                {
                    googleFree = false;
                    Log.d("ConfigFile", "Signing to GPG");
                    InitConnection();
                }
            } else {
                try {
                    config.createNewFile();

                    AlertDialog.Builder playGamesDia;
                    playGamesDia = new AlertDialog.Builder(this);
                    String errMsg;

                    errMsg = "Do you want to sign-in to Google Play Games?";
                    playGamesDia.setMessage(errMsg)
                            .setCancelable(false)
                            .setPositiveButton("Yes", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id)
                                {
                                    writeToFile("GPG", config);
                                    InitConnection();
                                }
                            })
                            .setNegativeButton("No", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                    writeToFile("noGPG", config);
                                    DownloadAssets();
                                }
                            });

                    AlertDialog alert = playGamesDia.create();
                    alert.setTitle("Google Sign-in");
                    alert.show();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }

    }

    @Override
    public void onDestroy() {
        // Clear DataStore using RxJava
        if( dataStore != null)
        {
            Log.d("Destroy Function", "Calling Clear Data");
            Completable.fromAction(dataStore.updateDataAsync(prefs -> {
                        prefs.toMutablePreferences().clear();
                        return Single.just(prefs);
                    })::blockingGet)
                    .subscribeOn(Schedulers.io())
                    .subscribe();

            // Clear Credential Manager state
            ClearCredentialStateRequest request = new ClearCredentialStateRequest("");

            if( credentialManager != null)
            {
                credentialManager.clearCredentialState(request, null);
            }
        }



        super.onDestroy();
    }




    private void writeToFile(String data,File file) {
        try {
            FileOutputStream stream = new FileOutputStream(file);
            try {
                stream.write(data.getBytes());
            } finally {
                stream.close();
            }
        }
        catch (IOException e) {
            e.printStackTrace();
        }
    }
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (handleKeyEventNative(keyCode, event.getAction())) {
            return true;
        }
        return super.onKeyDown(keyCode, event);
    }

    private native void nativeKeyPress(int keyCode);

    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {
        if (event.getAction() == KeyEvent.ACTION_DOWN) {
            nativeKeyPress(event.getKeyCode());
            return true;
        }
        return super.dispatchKeyEvent(event);
    }



    private String readFromFile(File file) {

        String ret = "";

        int length = (int) file.length();

        byte[] bytes = new byte[length];

        try {
            FileInputStream in = new FileInputStream(file);
            try {
                in.read(bytes);
            } finally {
                in.close();
            }

        }
        catch (IOException e) {
            e.printStackTrace();
        }
        ret = new String(bytes);

        return ret;
    }

    private void signInSilently()
    {
        int googlePlayServicesVersion = GooglePlayServicesUtil.isGooglePlayServicesAvailable(this);
        if (googlePlayServicesVersion != ConnectionResult.SUCCESS) {
            GoogleApiAvailability apiAvailability = GoogleApiAvailability.getInstance();
            apiAvailability.getErrorDialog(this, googlePlayServicesVersion, REQUEST_CODE).show();
        }

        // CredentialManager.
        Log.d("Credential Manager", "Sign in  Silently function called");
        credentialManager = CredentialManager.create(this);
        String hashedNonce = createHashedNonce();

        String client_id = "436965655818-13upb55ek77hp4daargv33pofk4bao2u.apps.googleusercontent.com";
        GetGoogleIdOption googleIdOption = new GetGoogleIdOption.Builder()
                .setFilterByAuthorizedAccounts(false)
                .setServerClientId(client_id)
                .setAutoSelectEnabled(false)
                .setNonce(hashedNonce)
                .build();

        GetCredentialRequest getCredRequest = new Builder()
                .addCredentialOption(googleIdOption)
                        .build();

        // Initialize Cancellation Signal
        CancellationSignal cancellationSignal = new CancellationSignal();

        // Initialize Executor
        Executor executor = Executors.newSingleThreadExecutor();

        credentialManager.getCredentialAsync
                (
                    // Use activity based context to avoid undefined
                    // system UI launching behavior
                    this,
                    getCredRequest,
                    cancellationSignal,
                    executor,
                    new CredentialManagerCallback<GetCredentialResponse, GetCredentialException>()
                    {
                        @Override
                        public void onResult(GetCredentialResponse result)
                        {
                        Log.d("Credential Manager", "Inside getCredentialAsync");
                        handleSignIn(result);
                        }
                        @Override
                        public void onError(GetCredentialException e)
                        {
                            Log.e("Credential Manager", "Error during credential retrieval: " + e.getMessage());
                        }

                    }
                );
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == REQUEST_CODE) {
            int result = GoogleApiAvailability.getInstance().isGooglePlayServicesAvailable(this);
            if (result == ConnectionResult.SUCCESS) {
                signInSilently();
                // Google Play services is now available
            }
        }
    }


    private String createHashedNonce() {
        //Create a Nonce
        String rawNonce = UUID.randomUUID().toString();
        byte[] bytes = rawNonce.getBytes();

        MessageDigest md;
        try
        {
            md = MessageDigest.getInstance("SHA-256");
        }
        catch (NoSuchAlgorithmException e)
        {
            throw new RuntimeException(e);
        }

        byte[] digest = md.digest(bytes);

        String hashedNonce = "";
        for (byte b : digest) {
            hashedNonce += String.format("02%x", b);
        }

        return hashedNonce;
    }

    private boolean isSignedIn() {
        // Assigning data store
        dataStore = new RxPreferenceDataStoreBuilder(this, "settings").build();

        // Get email
        Preferences.Key<String> emailKey = PreferencesKeys.stringKey("email");
        Flowable<String> emailFlow = dataStore.data().map(prefs -> prefs.get(emailKey));

        try {
            // Blocking call to get the email synchronously
            String email = emailFlow.blockingFirst(null);
            if (email != null && !email.isEmpty()) {
                Log.d("Sign in Result", "User is already signed in: " + email);
                return true;
            } else {
                Log.d("Sign in Result", "User is not signed in");
                return false;
            }
        } catch (Exception e) {
            Log.e("Error", "Failed to retrieve email", e);
            return false;
        }
    }



    public void handleSignIn(GetCredentialResponse result) {
        // Handle the successfully returned credential.
        Log.d("Credential Manager", "Insdie Handle Sign in function");

        Credential credential = result.getCredential();
        if (credential instanceof PublicKeyCredential) {
            String responseJson = ((PublicKeyCredential) credential).getAuthenticationResponseJson();
            Log.d("Credential Manager", "Got Response json Public Key Credential");

            // Share responseJson i.e. a GetCredentialResponse on your server to validate and authenticate
        } else if (credential instanceof PasswordCredential) {
            String username = ((PasswordCredential) credential).getId();
            String password = ((PasswordCredential) credential).getPassword();
            Log.d("Credential Manager", "Got Response json Password Credential");
            //Credential ExampleCustomCredential = new Credential();
            // Use id and password to send to your server to validate and authenticate
        } else if (credential instanceof CustomCredential) {
            if (credential.getType().equals(GoogleIdTokenCredential.TYPE_GOOGLE_ID_TOKEN_CREDENTIAL)) {
                try {
                    GoogleIdTokenCredential googleIdTokenCredential = GoogleIdTokenCredential.createFrom(credential.getData());
                    String id = googleIdTokenCredential.getIdToken();
                    Log.d("Sign in Received", "User id token " + id);

                    String email = googleIdTokenCredential.getId();
                    Log.d("Sign in Received", "User email " + email);
//                    // Extract the required credentials and complete the
//                    // authentication as per the federated sign in or any external
//                    // sign in library flow
                } catch (Exception e) {
//                    // Unlikely to happen. If it does, you likely need to update the
//                    // dependency version of your external sign-in library.
                    Log.e(TAG, "Failed to parse an ExampleCustomCredential", e);
                }
            }
        }
    }

        private void startSignInIntent()
    {

//        GoogleSignInClient signInClient = GoogleSignIn.getClient(this,
//                new GoogleSignInOptions.Builder(GoogleSignInOptions.DEFAULT_SIGN_IN)
//                        .requestServerAuthCode(client_id)//(getString(R.string.default_web_client_id))
//                        .requestIdToken(client_id)//(getString(R.string.default_web_client_id))
//                        .requestScopes(new Scope(Scopes.GAMES))
//                        .requestEmail()
//                        .requestProfile()
//                        .build());


//        Intent intent = signInClient.getSignInIntent();
//        startActivityForResult(intent, RC_SIGN_IN);
//        System.out.println("Signing in ...");
        }

        private void InitConnection ()
        {
            Log.d("Credential Manager", "Inside Init function");
            FirebaseAnalytics mFirebaseAnalytics = FirebaseAnalytics.getInstance(this);

            if (!isSignedIn()) {
                signInSilently();
            }
            mAuth = FirebaseAuth.getInstance();
            DownloadAssets();
        }

        private void DownloadAssets ()
        {

            File obbFile = new File(getObbDir(), "main." + BuildConfig.VERSION_CODE + ".com.example.test_android_kanji.obb");
            //File obbFile = new File(getObbDir(),"main."+"Test_Version"+ ".in.tuttifrutti.darkarta.abhq.full.obb");
            if (!obbFile.exists()) {

                final Intent intent = new Intent(MainActivity.this, AssetDownloader.class);
                MainActivity.this.startActivity(intent);
            } else {
                startGame();
            }
        }

    private void onConnected(GoogleSignInAccount googleSignInAccount) {
////        Log.d(TAG, "onConnected(): connected to Google APIs");
//
////        System.out.println("connected to Google APIs");
//
//        if(isGPG_Disabled)
//            return;
//
//        mAchievementsClient = PlayGames.getAchievementsClient(this);
//        mEventsClient = PlayGames.getEventsClient(this);
//        mPlayersClient = PlayGames.getPlayersClient(this);
//
//
//        // Set the greeting appropriately on main menu
//        mPlayersClient.getCurrentPlayer()
//                .addOnCompleteListener(new OnCompleteListener<Player>() {
//                    @Override
//                    public void onComplete(@NonNull Task<Player> task) {
//                        String displayName;
//                        if (task.isSuccessful()) {
//                            displayName = task.getResult().getDisplayName();
//
//                            System.out.println("Current Player is " + displayName);
//
//                        } else {
//                            Exception e = task.getException();
//                            handleException(e, getString(R.string.players_exception));
//                            displayName = "???";
//                        }
//                        //    mMainMenuFragment.setGreeting("Hello, " + displayName);
//                    }
//                });
//
//        // if we have accomplishments to push, push them
//        pushAccomplishments();
//        //Toast.makeText(this, getString(R.string.your_progress_will_be_uploaded), Toast.LENGTH_LONG).show();
//    }

//    private boolean isSignedIn() {
//        return GoogleSignIn.getLastSignedInAccount(this) != null;
    }

        public int AwardID = -1;
        private void pushAccomplishments () {

            if (isGPG_Disabled)
                return;

            if (!isSignedIn()) {
                // can't push to the cloud, try again later
                return;
            }

            System.out.println(AccomplishmentsCompleted);
            System.out.println(AccomplishmentsUnlocked);
            BitSet tempAccomplishmentsUnlocked = AccomplishmentsUnlocked;
            if (!AccomplishmentsUnlocked.equals(AccomplishmentsCompleted)) {
                System.out.println("Not equal");
                System.out.println(tempAccomplishmentsUnlocked);
                tempAccomplishmentsUnlocked.xor(AccomplishmentsCompleted);

                System.out.println(AccomplishmentsUnlocked);
                System.out.println(tempAccomplishmentsUnlocked);
                AccomplishmentsUnlocked.and(tempAccomplishmentsUnlocked);
                System.out.println(AccomplishmentsUnlocked);
            }


            System.out.println("Achievement unlocked" + AwardID);
            switch (AwardID) {
                case 0:
                    mAchievementsClient.unlock(getString(R.string.achievement_drona));
                    break;
                case 1:
                    mAchievementsClient.unlock(getString(R.string.achievement_na_sucita));
                    break;
                case 2:
                    mAchievementsClient.unlock(getString(R.string.achievement_na_sucita_3));
                    break;
                case 3:
                    mAchievementsClient.unlock(getString(R.string.achievement_na_sucita_5));
                    break;
                case 4:
                    mAchievementsClient.unlock(getString(R.string.achievement_na_sucita_10));
                    break;
                case 5:
                    mAchievementsClient.unlock(getString(R.string.achievement_arjuna));
                    break;
                case 6:
                    mAchievementsClient.unlock(getString(R.string.achievement_vayu));
                    break;
                case 7:
                    mAchievementsClient.unlock(getString(R.string.achievement_vayu_2));
                    break;
                case 8:
                    mAchievementsClient.unlock(getString(R.string.achievement_vayu_3));
                    break;
                case 9:
                    mAchievementsClient.unlock(getString(R.string.achievement_sama_kanta));
                    break;
                case 10:
                    mAchievementsClient.unlock(getString(R.string.achievement_agni));
                    break;
                case 11:
                    mAchievementsClient.unlock(getString(R.string.achievement_agni_2));
                    break;
                case 12:
                    mAchievementsClient.unlock(getString(R.string.achievement_agni_3));
                    break;
                case 13:
                    mAchievementsClient.unlock(getString(R.string.achievement_guru));
                    break;
                case 14:
                    mAchievementsClient.unlock(getString(R.string.achievement_nirmoktr));
                    break;
                /*
            case "Award_Pick5_WithinTime":
                mAchievementsClient.unlock(getString(R.string.achievement_HO_Without_Hint_1));
                break;
                 */
                case 16:
                    mAchievementsClient.unlock(getString(R.string.achievement_chanakya));
                    break;
                case 17:
                    mAchievementsClient.unlock(getString(R.string.achievement_ma_cara));
                    break;
                case 18:
                    mAchievementsClient.unlock(getString(R.string.achievement_hop_pati));
                    break;
                case 19:
                    mAchievementsClient.unlock(getString(R.string.achievement_tartariti));
                    break;
                case 20:
                    mAchievementsClient.unlock(getString(R.string.achievement_karma));
                    break;
                case 21:
                    mAchievementsClient.unlock(getString(R.string.achievement_karma_2));
                    break;
                case 22:
                    mAchievementsClient.unlock(getString(R.string.achievement_prarambha));
                    break;
                case 23:
                    mAchievementsClient.unlock(getString(R.string.achievement_devarupin));
                    break;
                case 24:
                    mAchievementsClient.unlock(getString(R.string.achievement_avara_cara));
                    break;
                case 25:
                    mAchievementsClient.unlock(getString(R.string.achievement_siddhi));
                    break;
                case 26:
                    mAchievementsClient.unlock(getString(R.string.achievement_udghatin));
                    break;
                case 27:
                    mAchievementsClient.unlock(getString(R.string.achievement_upalambhaka));
                    break;
                case 28:
                    mAchievementsClient.unlock(getString(R.string.achievement_sita));
                    break;
                case 29:
                    mAchievementsClient.unlock(getString(R.string.achievement_matr_raksa));
                    break;
                case 30:
                    mAchievementsClient.unlock(getString(R.string.achievement_drastta));
                    break;
                case 31:
                    mAchievementsClient.unlock(getString(R.string.achievement_garuda));
                    break;
                case 32:
                    mAchievementsClient.unlock(getString(R.string.achievement_adhimukti));
                    break;
                case 33:
                    mAchievementsClient.unlock(getString(R.string.achievement_samaya));
                    break;
                case 34:
                    mAchievementsClient.unlock(getString(R.string.achievement_pathaka));
                    break;
                case 35:
                    mAchievementsClient.unlock(getString(R.string.achievement_kaushik));
                    break;
                case 36:
                    mAchievementsClient.unlock(getString(R.string.achievement_pathika));
                    break;
                case 37:
                    mAchievementsClient.unlock(getString(R.string.achievement_rakshak));
                    break;
                case 38:
                    mAchievementsClient.unlock(getString(R.string.achievement_damaka));
                    break;
                case 39:
                    mAchievementsClient.unlock(getString(R.string.achievement_vigraha));
                    break;
                case 40:
                    mAchievementsClient.unlock(getString(R.string.achievement_raksaka));
                    break;
                default:
                    break;
            }

        }

        private void handleException (Exception e, String details){
            int status = 0;

            if (e instanceof ApiException) {
                ApiException apiException = (ApiException) e;
                status = apiException.getStatusCode();
            }

            String message = getString(R.string.status_exception_error, details, status, e);


            new AlertDialog.Builder(MainActivity.this)
                    .setMessage(message)
                    .setNeutralButton(android.R.string.ok, null)
                    .show();
        }

        FirebaseUser firebaseUser;

        private void firebaseAuthWithGoogle ( final GoogleSignInAccount acct){
            Log.d(TAG, "firebaseAuthWithGoogle:" + acct.getId());

            AuthCredential credential = GoogleAuthProvider.getCredential(acct.getIdToken(), null);

            mAuth.signInWithCredential(credential)
                    .addOnCompleteListener(this, new OnCompleteListener<AuthResult>() {
                        @Override
                        public void onComplete(Task<AuthResult> task) {
                            if (task.isSuccessful()) {
                                // Sign in success, update UI with the signed-in user's information
                                Log.d(TAG, "signInWithCredential:success");

                                firebaseUser = mAuth.getCurrentUser();
                                firebaseDatabaseSync(acct);

                            } else {
                                // If sign in fails, display a message to the user.
                                Log.w(TAG, "signInWithCredential:failure", task.getException());
                            }
                        }
                    });
        }

        private boolean userExist, gotUserData, isGameUnlocked, isGPG_Disabled;
        int noOfHints = -777;
        public void firebaseDatabaseSync ( final GoogleSignInAccount acct)
        {
            Log.d(TAG, "firebaseDatabaseSync:" + acct.getId());

            fireDataRef = FirebaseDatabase.getInstance().getReference().child("UserInfo");

            userExist = false;
            gotUserData = false;

            FirebaseDatabase.getInstance().getReference()
                    .child("UserInfo").child(firebaseUser.getUid())
                    .addListenerForSingleValueEvent(new ValueEventListener() {
                        @Override
                        public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                            UserInfo _user = dataSnapshot.getValue(UserInfo.class);
                            gotUserData = true;
                            if (_user != null) {
                                userExist = true;
                                System.out.println("old user");
                                user = _user;
                                noOfHints = user.getHints();
                                isGameUnlocked = user.isUnlocked();
                            } else {
                                userExist = false;
                                System.out.println("new user");
                                user = new UserInfo(firebaseUser.getUid(), acct.getDisplayName(), acct.getEmail(), 25, false);
                                fireDataRef.child(firebaseUser.getUid()).setValue(user);
                                noOfHints = user.getHints();
                                isGameUnlocked = user.isUnlocked();
                            }
                        }

                        @Override
                        public void onCancelled(@NonNull DatabaseError error) {

                        }
                    });
        }

        private void onDisconnected () {
            Log.d(TAG, "onDisconnected()");

            mAchievementsClient = null;
            //    private EventsClient mEventsClient;
            PlayersClient mPlayersClient = null;
        }

//    private void handleSignInResult(Task<GoogleSignInAccount> completedTask) {
//        try {
//            GoogleSignInAccount account = completedTask.getResult(ApiException.class);
//
//            //onConnected(account);
//
//            //firebaseAuthWithGoogle(account);
//        } catch (ApiException e) {
//            // The ApiException status code indicates the detailed failure reason.
//            // Please refer to the GoogleSignInStatusCodes class reference for more information.
//            Log.w(TAG, "signInResult:failed code=" + e.getStatusCode());
//        }
//    }

        String gameVersion = "";
        private void startGame ()
        {
            Log.d(TAG, "Starting Game");

            final String deviceName = Build.MODEL;
            int LanguageID = 0;
            final String PublicDCIMFolder = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DCIM).toString();
            boolean skipIntro = false;

            if (BuildConfig.DEBUG)
                skipIntro = true;
            //skipIntro = false;

            boolean cameraGranted = false;
            KanjiGameLib.setConfigVals(BuildConfig.VERSION_CODE, BuildConfig.VERSION_NAME, BuildConfig.CURRENT_MAIN_OBB_VERSION_IN_STORE, deviceName, BuildConfig.DEBUG, BuildConfig.GAME_DEBUG_RES, skipIntro, BuildConfig.GAME_SURVEY_BUILD, LanguageID, this.getResources().getInteger(R.integer.vid_res), gameVersion, this.getResources().getString(R.string.survey_url), this.getResources().getString(R.string.review_url), cameraGranted, PublicDCIMFolder);

        }



    /*
     * A native method that is implemented by the 'test_android_kanji' native library,
     * which is packaged with this application.
     */
    //public native String stringFromJNI();
}

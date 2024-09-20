plugins {
    alias(libs.plugins.android.application)
}

android {
    namespace = "com.example.test_android_kanji"
    compileSdk = 34


        buildFeatures {
            buildConfig = true
        }

    defaultConfig {
        applicationId = "com.example.test_android_kanji"
        minSdk = 29
        targetSdk = 34
        versionCode = 1
        versionName = "1.0"

        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"

        buildConfigField("int","CURRENT_MAIN_OBB_VERSION_IN_STORE","1")
        buildConfigField("long","CURRENT_MAIN_OBB_VERSION_FILESIZE","492596890L")
        buildConfigField("boolean","GAME_DEBUG_ENABLE","false")
        buildConfigField("int","GAME_DEBUG_RES","0")
        buildConfigField("boolean","GAME_SPLASH_SKIP","false")
        buildConfigField("boolean","GAME_SURVEY_BUILD","false")
    }

    buildTypes {

        release {
            isDebuggable = true;
            isMinifyEnabled = false
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }

    }
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_1_8
        targetCompatibility = JavaVersion.VERSION_1_8
    }
    externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
            version = "3.22.1"
        }
    }
    buildFeatures {
        viewBinding = true
    }
}

dependencies {

    implementation (libs.github.fetch2okhttp)
    implementation(libs.fetch2)
    implementation(libs.appcompat)
    implementation(libs.material)
    implementation(libs.constraintlayout)
    implementation(libs.play.services.tasks)
    implementation(libs.firebase.storage)
    testImplementation(libs.junit)
    androidTestImplementation(libs.ext.junit)
    androidTestImplementation(libs.espresso.core)
}
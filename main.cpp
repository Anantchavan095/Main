#include <jni.h>
#include <string>
#include <algorithm>
#include <string>
#include <vector>
#include <string.h>
#include <pthread.h>
#include "imgui/stb_image.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_android.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/Font.h"
#include "imgui/Icon.h"
#include "imgui/Iconcpp.h"
#include "imgui/imgui_internal.h"
#include "imgui/Notify/ImGuiNotify.hpp"
#include <Helper/define.h>
#include "Helper/ImguiPP.h"
#include "Helper/fake_dlfcn.h"
#include "Helper/Includes.h"
#include "Helper/plthook.h"
#include "Helper/json.hpp"
#include "Helper/StrEnc.h"
#include "Helper/Spoof.h"
#include "Helper/Tools.h"


#include <iostream>
#include <map>
#include <vector>
#include <mutex>
#include <unordered_set>
#include <cstring>
#include <string>
#include <pthread.h>
#include <GLES2/gl2.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <GLES/glplatform.h>

#include "patch/MemoryPatch.h"


#include <cstring>
#include <curl/curl.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include "Helper/json.hpp"
#include "Includes/VipIncludes.h"
#include <xhook/xhook.h>
#include "oxorany.h"

#define MasterMaskShader1 oxorany("Master_Mask_Base/DZ_Shirt_01_XQYS/TGPUSkinVertexFactoryfalse")
#define MasterMaskShader2 oxorany("Master_Mask_Base/DZ_Shirt_01_XQYS/TGPUSkinVertexFactorytrue")

using json = nlohmann::json;

android_app *g_App = nullptr;

bool center = true;
bool outlined = true;
float accent_color[4] = {0.300f, 0.220f, 0.750f, 1.000f};

int gScreenWidth;
int gScreenHeight;


bool initImGui = false;
int screenWidth = -1, glWidth, screenHeight = -1, glHeight;
int counterWidth = 0;
float density = -1;
json items_data;

bool g_Initialized = false;
ImGuiWindow* g_window = NULL;

std::string expiretime = "";
std::string exp_time;
std::string g_Token, g_Auth;
bool bValid = false;
#define SLEEP_TIME 1000LL / 120LL
static char s[64];
	
// ======================================================================== //

std::map<int, bool> Items;
std::map<int, float *> ItemColors;

enum EAimTrigger {
None = 1,
Shooting = 2,
Scoping = 3,
Both = 4
};

// hiremin



static int isLineUi = 1;
static int isAimTrigger = 1;
bool isInfo, isLine, isBone, isBox3D, isGrenade, isAlert, isCars, isBox, isBoxItems;
bool isAimbot, isHideFov, isAimKnocked, isAimSettings, isIgnoreBot, isAimLine, isVisCheck;
static float isAimRecoil = 1, isAimDistance = 70, isFovSize = 25;
bool isShowSkin;
bool isView;
bool Bypass;
bool Enable;   
int Meter;
bool Pov;
bool Pred;
bool Recoil;
bool Aimbot;
int Cross;
int Position;
float Recc;
EAimTrigger Trigger;
bool IgnoreKnocked;
bool VisCheck;
        bool IgnoreBots;
bool Bullet;
bool Armp120Fps;
		



bool SilentAim;

bool Ipad;
 

// ======================================================================== //
#define CREATE_COLOR(r, g, b, a) new float[4] {(float)r, (float)g, (float)b, (float)a};
// ======================================================================== //
std::map<int, bool> itemConfig;
std::map<std::string, u_long> Config;


struct sRegion {
    uintptr_t start, end;
};


std::mutex playerProgramsMutex;
std::mutex playerShaderMutex;
std::unordered_set<GLuint> playerPrograms;
GLuint playerVertexShader;
GLuint playerMaskShader;

// Original function pointers
void (*oglDrawElements)(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
void (*oglShaderSource)(GLuint shader, GLsizei count, const GLchar **string, const GLint *length);
void (*oglAttachShader)(GLuint program, GLuint shader);

// Hooked function for glDrawElements
void _glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices) {
 if (isInfo) {
        GLint c_prog;
        glGetIntegerv(GL_CURRENT_PROGRAM, &c_prog);

        {
            std::lock_guard<std::mutex> lock(playerProgramsMutex);
            if (playerPrograms.find(c_prog) == playerPrograms.end()) {
                oglDrawElements(mode, count, type, indices);
                return;
            }
        }

        GLint c_id = glGetUniformLocation(c_prog, "vu_u");
        if (c_id != -1) {
            GLfloat depthRange[2];
            glGetFloatv(GL_DEPTH_RANGE, depthRange);
            GLboolean colorWriteMask[4];
            glGetBooleanv(GL_COLOR_WRITEMASK, colorWriteMask);
            GLboolean blend;
            glGetBooleanv(GL_BLEND, &blend);

            if (!blend) {
                glEnable(GL_BLEND);
            }
            glDepthRangef(1.0f, 0.0f);
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

            if (Config["ESP::CHAMS"]) {
                GLint blendSrc, blendDst;
                glGetIntegerv(GL_BLEND_SRC, &blendSrc);
                glGetIntegerv(GL_BLEND_DST, &blendDst);
                GLfloat blendColor[4];
                glGetFloatv(GL_BLEND_COLOR, blendColor);
                glBlendFunc(GL_CONSTANT_COLOR, GL_ZERO);
                glBlendColor(166.0 / 255.0, 255.0 / 255.0, 0.0, 0.8);
                oglDrawElements(mode, count, type, indices);
                glBlendColor(blendColor[0], blendColor[1], blendColor[2], blendColor[3]);
                glBlendFunc(blendSrc, blendDst);
            } else {
                oglDrawElements(mode, count, type, indices);
            }

            glDepthRangef(depthRange[0], depthRange[1]);
            glColorMask(colorWriteMask[0], colorWriteMask[1], colorWriteMask[2], colorWriteMask[3]);

            if (!blend) {
                glDisable(GL_BLEND);
            }
            return;
        }
    }
    oglDrawElements(mode, count, type, indices);
}

// Hooked function for glShaderSource
void _glShaderSource(GLuint shader, GLsizei count, const GLchar **string, const GLint *length) {
    if (!string || !*string || count == 0) {
        return oglShaderSource(shader, count, string, length);
    }

    const char* shaderSource = *string;

  /*  if (strstr(shaderSource, "Master_VH_IBL_SeparateSDW/TGPUSkinVertexFactoryfalse") ||
        strstr(shaderSource, "Master_VH_IBL_SeparateSDW/TGPUSkinVertexFactorytrue")) {
        GLenum shaderType = 0;
        glGetShaderiv(shader, GL_SHADER_TYPE, reinterpret_cast<GLint*>(&shaderType));
        if (shaderType == GL_FRAGMENT_SHADER) {
            std::lock_guard<std::mutex> lock(playerShaderMutex);
            playerMaskShader = shader;
            *string = newPlayerMaskShaderRed;
        }
    }
*/
    if (strstr(shaderSource, MasterMaskShader1) || strstr(shaderSource, MasterMaskShader2)) {GLenum shaderType = 0;
        glGetShaderiv(shader, GL_SHADER_TYPE, reinterpret_cast<GLint*>(&shaderType));
        if (shaderType == GL_FRAGMENT_SHADER) {
            std::lock_guard<std::mutex> lock(playerShaderMutex);
            playerMaskShader = shader;
          //  *string = newPlayerMaskShaderG;
        }
    }

    return oglShaderSource(shader, count, string, length);
}

// Hooked function for glAttachShader
void _glAttachShader(GLuint program, GLuint shader) {
    {
        std::lock_guard<std::mutex> lock(playerShaderMutex);
        if (shader == playerVertexShader || shader == playerMaskShader) {
            std::lock_guard<std::mutex> progLock(playerProgramsMutex);
            playerPrograms.insert(program);
        }
    }
    return oglAttachShader(program, shader);
}


#include <unistd.h> // for sleep
#include <sys/stat.h> // for stat

bool executeCommand(const std::string& command) {
    int result = system(command.c_str());
    return (result == 0);
}

bool removeFile(const std::string& filePath) {
    std::string command = "rm " + filePath;
    return executeCommand(command);
}

bool removeDirectory(const std::string& dirPath) {
    std::string command = "rm -rf " + dirPath;
    return executeCommand(command);
}

bool changeFilePermissions(const std::string& filePath, const std::string& permissions) {
    int result = chmod(filePath.c_str(), strtol(permissions.c_str(), nullptr, 8));
    return (result == 0);
}

bool createFile(const std::string& filePath) {
    std::ofstream file(filePath);
    return file.good();
}

bool fileExists(const std::string& filePath) {
    struct stat buffer;
    return (stat(filePath.c_str(), &buffer) == 0);
}

// Function to fix PUBG termination
void fixPUBGTermination() {
    std::vector<std::string> filesToRemove = {
        // "/data/media/0/Android/data/com.pubg.imobile/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/SrcVersion.ini",
        // "/storage/emulated/0/Android/data/com.pubg.imobile/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/SrcVersion.ini",
        "/data/app/*/com.pubg.imobile*/lib/arm/libCrashKit.so",
        "/data/app/com.pubg.imobile*/lib/arm/libCrashKit.so",
        "/data/data/com.pubg.imobile/lib/libCrashKit.so",
        "/data/data/com.pubg.imobile/files",
        "/data/data/com.pubg.imobile/files/ano_tmp",
        "/storage/emulated/0/Android/data/com.pubg.imobile/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/Paks/game_patch_3.2.0.18763.pak",
        "/storage/emulated/0/Android/data/com.pubg.imobile/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/Paks/game_patch_3.2.0.18761.pak",
        "/storage/emulated/0/Android/data/com.pubg.imobile/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/Paks/game_patch_3.2.0.18759.pak",
        "/storage/emulated/0/Android/data/com.pubg.imobile/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/Paks/game_patch_3.2.0.18757.pak",
        "/storage/emulated/0/Android/data/com.pubg.imobile/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/Paks/game_patch_3.2.0.18755.pak",
        "/storage/emulated/0/Android/data/com.pubg.imobile/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/Paks/game_patch_3.2.0.18766.pak",
        "/storage/emulated/0/Android/data/com.pubg.imobile/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/Paks/game_patch_3.2.0.18765.pak",
        "/storage/emulated/0/Android/data/com.pubg.imobile/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/Paks/game_patch_3.2.0.18770.pak",
        "/storage/emulated/0/Android/data/com.pubg.imobile/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/Paks/game_patch_3.2.0.18768.pak",
        "/storage/emulated/0/Android/data/com.pubg.imobile/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/Paks/1002112598_3078_3.2.0.18770_20240612031547_578852825_cures.ifs.res",
        "/data/data/com.pubg.imobile/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/Paks/game_patch_3.2.0.18763.pak",
        "/data/data/com.pubg.imobile/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/Paks/game_patch_3.2.0.18761.pak",
        "/data/data/com.pubg.imobile/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/Paks/game_patch_3.2.0.18759.pak",
        "/data/data/com.pubg.imobile/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/Paks/game_patch_3.2.0.18757.pak",
        "/data/data/com.pubg.imobile/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/Paks/game_patch_3.2.0.18755.pak",
        "/data/data/com.pubg.imobile/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/Paks/game_patch_3.2.0.18766.pak",
        "/data/data/com.pubg.imobile/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/Paks/game_patch_3.2.0.18765.pak",
        "/data/data/com.pubg.imobile/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/Paks/game_patch_3.2.0.18770.pak",
        "/data/data/com.pubg.imobile/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/Paks/game_patch_3.2.0.18768.pak",
        "/data/data/com.pubg.imobile/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/Paks/1002112598_3078_3.2.0.18770_20240612031547_578852825_cures.ifs.res"
    };

    for (const auto& filePath : filesToRemove) {
        if (fileExists(filePath)) {
            removeFile(filePath);
            usleep(1000000); // Sleep for 1 second
        }
    }

    changeFilePermissions("/data/media/0/Android/data/com.pubg.imobile/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/Paks", "555");
    usleep(1000000);

    createFile("/data/data/com.pubg.imobile/files/ano_tmp");
    usleep(1000000);
    changeFilePermissions("/data/data/com.pubg.imobile/files/ano_tmp", "000");
}

/*void fix10() {
    struct Patch {
        uintptr_t offset;
        std::string data;
    };

    Patch patches[] = {
        {0x3c4a18, "h00 20 70 47"},
        {0x3da6c2, "h00 20 70 47"},
        {0x3da6ec, "h00 20 70 47"},
        {0x3c4a14, "h00 20 70 47"},
        {0x3da6c2, "h00 20 70 47"},
        {0x23EA48, "h00 20 70 47"},
        {0x23EA38, "h00 20 70 47"},
        {0x23E9D8, "h00 20 70 47"},
        {0x23E9E0, "h00 20 70 47"},
        {0x23E9D0, "h00 20 70 47"},
        {0x23E990, "h00 20 70 47"},
        {0x23E978, "h00 20 70 47"},
        {0x23E968, "h00 20 70 47"},
        {0x23E958, "h00 20 70 47"},
        {0x23E938, "h00 20 70 47"},
        {0x23E948, "h00 20 70 47"},
        {0x23E920, "h00 20 70 47"},
        {0x23E918, "h00 20 70 47"},
        {0x23E910, "h00 20 70 47"},
        {0x23BCE8, "h00 20 70 47"},
        {0x23BCF8, "h00 20 70 47"},
        {0x23BC98, "h00 20 70 47"},
        {0x23BCA0, "h00 20 70 47"}
    };

    uintptr_t baseAddr = 0x1000; // Example base address, this should be the base address of the loaded "libanogs.so"

    for (const auto& patch : patches) {
        uintptr_t address = baseAddr + patch.offset;
        // Write the patch to the memory address
        MemoryPatch::createWithHex("libanogs.so", address, patch.data).Modify();
    }
}
*/
/*#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include "oxorany.h"
#include "Includes/Logger.h"
//#include "Includes/obfuscate.h"
#include "Includes/Utils.h"

#define libTBlueData "libTBlueData.so"

//libUE4.so size 0x7E77C14
libanogs.so size 0x4F4A4
unsigned int libTBlueDataSize = 0x20354;
unsigned int libTBlueDataBase = 0;

int __fastcall sub_4F4EC(int result, int a2)
{
    int v2;
    int v3;
    int v5[5];

    *((unsigned char *)(a2 + 16)) = *((unsigned char *)(result + 1252));
    if (*((unsigned char *)(result + 1252)) != 1)
    {
        memset((void *)a2, 0, 0x10);
        sigaction(13, NULL, (struct sigaction *)a2);
        v2 = *((unsigned int *)(a2 + 8)); // @CHEAT_BYPASS
        v3 = *((unsigned int *)(a2 + 12)); // @CHEAT_BYPASS
        v5[1] = *((unsigned int *)(a2 + 4)); // @CHEAT_BYPASS
        v5[2] = v2; // @CHEAT_BYPASS
        v5[3] = v3; // @CHEAT_BYPASS
        v5[0] = 1; // @CHEAT_BYPASS
        return sigaction(13, (const struct sigaction *)v5, NULL);
    }
    return result;
}

void *MonsterCheat(void *) {
do {
sleep(1);
} while (!isLibraryLoaded("libTBlueData.so"));
HOOK_LIB("libTBlueData.so", "0x4F518", memset ); //@CHEAT_BYPASS
return NULL;
}

*/
int PlaySoundYaserMod(const char* url) {
    JavaVM* java_vm = g_App->activity->vm;
    JNIEnv* java_env = NULL;
    jint jni_return = java_vm->GetEnv((void**)&java_env, JNI_VERSION_1_6);
    if (jni_return == JNI_EDETACHED) {
    if (java_vm->AttachCurrentThread(&java_env, NULL) != JNI_OK) {
        return -1;
    }
    } else if (jni_return == JNI_EVERSION) {
        return -1;
    }
    jclass sound = java_env->FindClass("android/media/MediaPlayer");
    if (sound == NULL) {
        return -3;
    }
    jmethodID create_method_id = java_env->GetMethodID(sound, "<init>", "()V");
    if (create_method_id == NULL) {
        return -4;
    }
    jobject media_player_obj = java_env->NewObject(sound, create_method_id);
    if (media_player_obj == NULL) {
        return -5;
    }
    jmethodID set_data_source_method_id = java_env->GetMethodID(sound, "setDataSource", "(Ljava/lang/String;)V");
    if (set_data_source_method_id == NULL) {
        return -6;
    }
    jstring url_str = java_env->NewStringUTF(url);
    java_env->CallVoidMethod(media_player_obj, set_data_source_method_id, url_str);
    jmethodID prepare_method_id = java_env->GetMethodID(sound, "prepare", "()V");
    if (prepare_method_id == NULL) {
        return -7;
    }
    java_env->CallVoidMethod(media_player_obj, prepare_method_id);
    jmethodID start_method_id = java_env->GetMethodID(sound, "start", "()V");
    if (start_method_id == NULL) {
        return -8;
    }
    java_env->CallVoidMethod(media_player_obj, start_method_id);
    java_env->DeleteLocalRef(sound);
    java_env->DeleteLocalRef(media_player_obj);
    java_env->DeleteLocalRef(url_str);
    if (java_vm->DetachCurrentThread() != JNI_OK) {
        return -1;
    }
    return 0;
}
	extern "C" {
		JNIEXPORT void JNICALL Java_com_xelahot_pubgm_GLES3JNIView_init(JNIEnv* env, jclass cls);
        JNIEXPORT void JNICALL Java_com_xelahot_pubgm_GLES3JNIView_resize(JNIEnv* env, jobject obj, jint width, jint height);
        JNIEXPORT void JNICALL Java_com_xelahot_pubgm_GLES3JNIView_step(JNIEnv* env, jobject obj);
	    JNIEXPORT void JNICALL Java_com_xelahot_pubgm_GLES3JNIView_imgui_Shutdown(JNIEnv* env, jobject obj);
	    JNIEXPORT void JNICALL Java_com_xelahot_pubgm_GLES3JNIView_MotionEventClick(JNIEnv* env, jobject obj,jboolean down,jfloat PosX,jfloat PosY);
	    JNIEXPORT jstring JNICALL Java_com_xelahot_pubgm_GLES3JNIView_getWindowRect(JNIEnv *env, jobject thiz);
	    JNIEXPORT void JNICALL Java_com_xelahot_pubgm_GLES3JNIView_real(JNIEnv* env, jobject obj, jint width, jint height);
	};
	
	namespace Settings {
    static int Tab = 1;
    }
	
	ImVec4 to_vec4(float r, float g, float b, float a) {
    return ImVec4(r / 255.0, g / 255.0, b / 255.0, a / 255.0);
    }

    JNIEXPORT void JNICALL Java_com_xelahot_pubgm_GLES3JNIView_init(JNIEnv* env, jclass cls) {

		if (g_Initialized) 
			return ;
			
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle *style = &ImGui::GetStyle();
style->FramePadding = ImVec2(4, 2);
style->ItemSpacing = ImVec2(10, 2);
style->IndentSpacing = 12;
style->ScrollbarSize = 12;

style->WindowRounding = 12;
style->FrameRounding = 0;  // Butonları kare yapmak için köşe yuvarlama değeri 0 yapıldı
style->PopupRounding = 4;
style->ScrollbarRounding = 10;
style->GrabRounding = 4;
style->TabRounding = 4;
style->WindowTitleAlign = ImVec2(0.5f, 0.5f);
style->WindowMenuButtonPosition = ImGuiDir_Left;  // Menü düğmesi solda olacak şekilde ayarlandı

style->DisplaySafeAreaPadding = ImVec2(7, 7);
ImVec4 *colors = style->Colors;

ImGui::StyleColorsLight();  // Bu satır açık temayı uygular
ImGui_ImplAndroid_Init();    


		io.IniFilename = NULL;
	io.ConfigWindowsMoveFromTitleBarOnly = true;

    ImGui_ImplAndroid_Init();

	ImGui_ImplOpenGL3_Init(OBFUSCATE("#version 300 es"));

		static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
        ImFontConfig icons_config;

        ImFontConfig CustomFont;
        CustomFont.FontDataOwnedByAtlas = false;

        icons_config.MergeMode = true;
        icons_config.PixelSnapH = true;
        icons_config.OversampleH = 2.5;
        icons_config.OversampleV = 2.5;
	
	    io.FontGlobalScale = 1.2f;
	
        io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(Custom), sizeof(Custom), 30.f, &CustomFont);
        io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 30.0f, &icons_config, icons_ranges);
		
		
        ImFontConfig font_cfg;

        font_cfg.SizePixels = 20.0f;
        io.Fonts->AddFontDefault(&font_cfg);
        ImGui::GetStyle().ScaleAllSizes(3.0f); 
    
        g_Initialized=true;
}

	JNIEXPORT void JNICALL
	Java_com_xelahot_pubgm_GLES3JNIView_resize(JNIEnv* env, jobject obj, jint width, jint height) {
		glWidth = (int) width;
		glHeight = (int) height;
		glViewport(0, 0, width, height);
		ImGuiIO & io = ImGui::GetIO();
		io.ConfigWindowsMoveFromTitleBarOnly = false;
		io.IniFilename = NULL;
		ImGui::GetIO().DisplaySize = ImVec2((float)width, (float)height);
	}
	// Define global variables





// Define global variables for tab state and checkboxes
int tab = 0;
bool show = true;
bool Fps31 = false;

void BeginDraw() {
    if (!show) return; // If the window is closed, exit the function

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImVec2 windowSize = ImVec2(700.0f, 500.0f);
    ImVec2 windowPos = ImVec2(center.x - windowSize.x / 2, center.y - windowSize.y / 2);

    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Appearing);
    ImGui::SetNextWindowSize(windowSize); // Set width to 900 and height to 700
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.f);
    ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(0, 0, 0, 255));

    if (ImGui::Begin("THUNDER-MODS", &show, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
        g_window = ImGui::GetCurrentWindow(); // Add this line here

        ImGui::Columns(2, NULL, false); // Create 2 columns

        // Adjust column widths to reduce the spacing
        ImGui::SetColumnWidth(0, 140.0f); // Adjust the width of the first column (left menu)

        // Draw a thick line between the columns
        ImVec2 columnSeparatorStart = ImGui::GetCursorScreenPos();
        columnSeparatorStart.x += 140.0f;
        ImVec2 columnSeparatorEnd = columnSeparatorStart;
        columnSeparatorEnd.y += ImGui::GetWindowHeight();
        ImGui::GetWindowDrawList()->AddLine(columnSeparatorStart, columnSeparatorEnd, IM_COL32(0, 255, 0, 255), 3.0f);

        // Left column for tabs
        ImGui::BeginGroup();
const char* tabLabels[] = { "SYSTEM",  "WH" };
for (int i = 0; i < IM_ARRAYSIZE(tabLabels); ++i) {
    if (strcmp(tabLabels[i], "SYSTEM") == 0 || strcmp(tabLabels[i], "SKIN") == 0) {
        // Skip rendering the button for "SYSTEM" or "SKIN"
        ImGui::Spacing(); // Add spacing to maintain layout
        continue;
    }

    ImVec4 colorActive = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
    ImVec4 colorInactive = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
    ImGui::PushStyleColor(ImGuiCol_Button, tab == i ? colorActive : colorInactive);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colorActive);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, colorActive);
    if (ImGui::Button(tabLabels[i], ImVec2(100, 50))) tab = i;  // Update tab selection
    ImGui::PopStyleColor(3);
    ImGui::Spacing(); // Add spacing between buttons
}
ImGui::EndGroup();

        ImGui::NextColumn();

        // Right column for tab content
        ImGui::BeginGroup();
        if (tab == 0) {
            // SYSTEM tab content
            ImGui::Text("WELCOME TO VIP HACKS");
            
     //     ImGui::Checkbox("120 FPS", &Armp120Fps);
         

            // Make Reset buttons slightly bigger
            ImVec2 resetButtonSize = ImVec2(200, 60);
            ImVec4 blueColor = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
            ImGui::PushStyleColor(ImGuiCol_Button, blueColor);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, blueColor);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, blueColor);

       //     ImGui::Button("Reset(1)", resetButtonSize);
            ImGui::SameLine();
      ///      ImGui::Button("Reset(2)", resetButtonSize);

            ImGui::PopStyleColor(3);

        } else if (tab == 1) {
            // ESP tab content
            ImGui::Checkbox("WH Active", &isInfo);
            ImGui::SameLine();
          /*  ImGui::Checkbox("ESP LINE", &isLine);
            ImGui::Checkbox("ESP BONE", &isBone);
            ImGui::SameLine();
            ImGui::Checkbox("BOX 3D", &isBox3D);
            ImGui::Checkbox("GRANADE", &isGrenade);
            ImGui::SameLine();
            ImGui::Checkbox("360° ALERT", &isAlert);
            ImGui::Checkbox("HIDE FOV", &isHideFov);
            ImGui::SameLine();
            ImGui::Checkbox("AIM-KNOCKED", &isAimKnocked);
            ImGui::TableNextColumn();
         //   ImGui::RadioButton("ESP Ui 1", &isESPUi, 1);
          //  ImGui::SameLine();
          //  ImGui::RadioButton("ESP Ui 2", &isESPUi, 2);
            ImGui::RadioButton("LINE 360°", &isLineUi, 1);
            ImGui::SameLine();
            ImGui::RadioButton("LINE 180°", &isLineUi, 2);
            ImGui::Checkbox("ESP CAR", &isCars);
            ImGui::SameLine();
            ImGui::Checkbox("ESP BOX", &isBox);
            ImGui::Checkbox("BOXITEMS", &isBoxItems);
            ImGui::SameLine();
            ImGui::Checkbox("120 FPS", &Armp120Fps);*/

        }
        ImGui::EndGroup();

        ImGui::Columns(1);
    }

    ImGui::End();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();
}
void EndDraw() {
    if (g_window) {
        g_window->DrawList->PushClipRectFullScreen();
    }
}
JNIEXPORT void JNICALL
Java_com_xelahot_pubgm_GLES3JNIView_step(JNIEnv* env, jobject obj) {
	ImGuiIO& io = ImGui::GetIO();
	
	static bool show_demo_window = false;
    static bool show_MainMenu_window = true;

	// Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(glWidth, glHeight);
    ImGui::NewFrame();
	
    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window) {
    ImGui::ShowDemoWindow(&show_demo_window);
	}
	
	if (show) {
	BeginDraw();
	}else{
	}
	
	if (show == false){
	if (ImGui::Begin(OBFUSCATE(" open" ), 0,ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove)) {
    g_window = ImGui::GetCurrentWindow();   
    if(ImGui::InvisibleButton("open", ImVec2(100, 100) )) {
    show = true;
    }
	ImGui::End();
	}
	}
	
	if (isShowSkin) {
	show = false;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.f);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.f);
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	ImGui::SetNextWindowSize(ImVec2(790.0, 1000.0));
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::Begin("BAGIRA-ENGINE (SKIN MENU)", &isShowSkin, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove); {
	g_window = ImGui::GetCurrentWindow();
	ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(0, 0, 0, 0));
	
	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
	ImGui::End();
	}
    }
	
	//EndDraw();
	
	//Rendering Notify
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
    ImGui::RenderNotifications();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(1);
	
    ImGui::Render();
	glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	
}

JNIEXPORT void JNICALL Java_com_xelahot_pubgm_GLES3JNIView_imgui_Shutdown(JNIEnv* env, jobject obj){
    if (!g_Initialized)
        return;
     // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplAndroid_Shutdown();
    ImGui::DestroyContext();
    g_Initialized=false;
}

JNIEXPORT void JNICALL Java_com_xelahot_pubgm_GLES3JNIView_MotionEventClick(JNIEnv* env, jobject obj,jboolean down,jfloat PosX,jfloat PosY){
	ImGuiIO & io = ImGui::GetIO();
	io.MouseDown[0] = down;
	io.MousePos = ImVec2(PosX,PosY);
}

JNIEXPORT jstring JNICALL Java_com_xelahot_pubgm_GLES3JNIView_getWindowRect(JNIEnv *env, jobject thiz) {
    // TODO: 实现 getWindowSizePos()
    char result[256]="0|0|0|0";
    if(g_window){
        sprintf(result,"%d|%d|%d|%d",(int)g_window->Pos.x,(int)g_window->Pos.y,(int)g_window->Size.x,(int)g_window->Size.y);
    }
    return env->NewStringUTF(result);
}

// ======================================================================== //

#include <pthread.h>
#include <dlfcn.h>


void* main_thread(void *) {
    fixPUBGTermination();
    // fix10();

    // Enable xhook debugging
    xhook_enable_debug(1);

    // Register hooks
    void* funcAddress;
    char *functionNames[] = {"glDrawElements", "glShaderSource", "glAttachShader"};
    void *hookedFunctions[] = {(void*)_glDrawElements, (void*)_glShaderSource, (void*)_glAttachShader};
    void **originalFunctions[] = {(void**)&oglDrawElements, (void**)&oglShaderSource, (void**)&oglAttachShader};

    // Loop through the functions to register
    for (int i = 0; i < sizeof(functionNames) / sizeof(functionNames[0]); i++) {
        funcAddress = dlsym(RTLD_DEFAULT, functionNames[i]);
        if (funcAddress) {
            xhook_register(".*\\.so$", functionNames[i], hookedFunctions[i], originalFunctions[i]);
        }
    }

    // Refresh hooks
    xhook_refresh(0);

    return NULL;
}

// Unhook functions
void unhookFunctions() {
    xhook_clear();
}

// Definitions
void (*oxorany)();
void _oxorany() {
    // Your code for oxorany
}



const char *GetAndroidID(JNIEnv *env, jobject context) {
    jclass contextClass = env->FindClass(/*android/content/Context*/ StrEnc("`L+&0^[S+-:J^$,r9q92(as", "\x01\x22\x4F\x54\x5F\x37\x3F\x7C\x48\x42\x54\x3E\x3B\x4A\x58\x5D\x7A\x1E\x57\x46\x4D\x19\x07", 23).c_str());
    jmethodID getContentResolverMethod = env->GetMethodID(contextClass, /*getContentResolver*/ StrEnc("E8X\\7r7ys_Q%JS+L+~", "\x22\x5D\x2C\x1F\x58\x1C\x43\x1C\x1D\x2B\x03\x40\x39\x3C\x47\x3A\x4E\x0C", 18).c_str(), /*()Landroid/content/ContentResolver;*/ StrEnc("8^QKmj< }5D:9q7f.BXkef]A*GYLNg}B!/L", "\x10\x77\x1D\x2A\x03\x0E\x4E\x4F\x14\x51\x6B\x59\x56\x1F\x43\x03\x40\x36\x77\x28\x0A\x08\x29\x24\x44\x33\x0B\x29\x3D\x08\x11\x34\x44\x5D\x77", 35).c_str());
    jclass settingSecureClass = env->FindClass(/*android/provider/Settings$Secure*/ StrEnc("T1yw^BCF^af&dB_@Raf}\\FS,zT~L(3Z\"", "\x35\x5F\x1D\x05\x31\x2B\x27\x69\x2E\x13\x09\x50\x0D\x26\x3A\x32\x7D\x32\x03\x09\x28\x2F\x3D\x4B\x09\x70\x2D\x29\x4B\x46\x28\x47", 32).c_str());
    jmethodID getStringMethod = env->GetStaticMethodID(settingSecureClass, /*getString*/ StrEnc("e<F*J5c0Y", "\x02\x59\x32\x79\x3E\x47\x0A\x5E\x3E", 9).c_str(), /*(Landroid/content/ContentResolver;Ljava/lang/String;)Ljava/lang/String;*/ StrEnc("$6*%R*!XO\"m18o,0S!*`uI$IW)l_/_knSdlRiO1T`2sH|Ouy__^}%Y)JsQ:-\"(2_^-$i{?H", "\x0C\x7A\x4B\x4B\x36\x58\x4E\x31\x2B\x0D\x0E\x5E\x56\x1B\x49\x5E\x27\x0E\x69\x0F\x1B\x3D\x41\x27\x23\x7B\x09\x2C\x40\x33\x1D\x0B\x21\x5F\x20\x38\x08\x39\x50\x7B\x0C\x53\x1D\x2F\x53\x1C\x01\x0B\x36\x31\x39\x46\x0C\x15\x43\x2B\x05\x30\x15\x41\x43\x46\x55\x70\x0D\x59\x56\x00\x15\x58\x73", 71).c_str());

    auto obj = env->CallObjectMethod(context, getContentResolverMethod);
    auto str = (jstring) env->CallStaticObjectMethod(settingSecureClass, getStringMethod, obj, env->NewStringUTF(/*android_id*/ StrEnc("ujHO)8OfOE", "\x14\x04\x2C\x3D\x46\x51\x2B\x39\x26\x21", 10).c_str()));
    return env->GetStringUTFChars(str, 0);
}

const char *GetDeviceModel(JNIEnv *env) {
    jclass buildClass = env->FindClass(/*android/os/Build*/ StrEnc("m5I{GKGWBP-VOxkA", "\x0C\x5B\x2D\x09\x28\x22\x23\x78\x2D\x23\x02\x14\x3A\x11\x07\x25", 16).c_str());
    jfieldID modelId = env->GetStaticFieldID(buildClass, /*MODEL*/ StrEnc("|}[q:", "\x31\x32\x1F\x34\x76", 5).c_str(), /*Ljava/lang/String;*/ StrEnc(".D:C:ETZ1O-Ib&^h.Y", "\x62\x2E\x5B\x35\x5B\x6A\x38\x3B\x5F\x28\x02\x1A\x16\x54\x37\x06\x49\x62", 18).c_str());

    auto str = (jstring) env->GetStaticObjectField(buildClass, modelId);
    return env->GetStringUTFChars(str, 0);
}

const char *GetDeviceBrand(JNIEnv *env) {
    jclass buildClass = env->FindClass(/*android/os/Build*/ StrEnc("0iW=2^>0zTRB!B90", "\x51\x07\x33\x4F\x5D\x37\x5A\x1F\x15\x27\x7D\x00\x54\x2B\x55\x54", 16).c_str());
    jfieldID modelId = env->GetStaticFieldID(buildClass, /*BRAND*/ StrEnc("@{[FP", "\x02\x29\x1A\x08\x14", 5).c_str(), /*Ljava/lang/String;*/ StrEnc(".D:C:ETZ1O-Ib&^h.Y", "\x62\x2E\x5B\x35\x5B\x6A\x38\x3B\x5F\x28\x02\x1A\x16\x54\x37\x06\x49\x62", 18).c_str());

    auto str = (jstring) env->GetStaticObjectField(buildClass, modelId);
    return env->GetStringUTFChars(str, 0);
}

const char *GetPackageName(JNIEnv *env, jobject context) {
    jclass contextClass = env->FindClass(/*android/content/Context*/ StrEnc("`L+&0^[S+-:J^$,r9q92(as", "\x01\x22\x4F\x54\x5F\x37\x3F\x7C\x48\x42\x54\x3E\x3B\x4A\x58\x5D\x7A\x1E\x57\x46\x4D\x19\x07", 23).c_str());
    jmethodID getPackageNameId = env->GetMethodID(contextClass, /*getPackageName*/ StrEnc("YN4DaP)!{wRGN}", "\x3E\x2B\x40\x14\x00\x33\x42\x40\x1C\x12\x1C\x26\x23\x18", 14).c_str(), /*()Ljava/lang/String;*/ StrEnc("VnpibEspM(b]<s#[9cQD", "\x7E\x47\x3C\x03\x03\x33\x12\x5F\x21\x49\x0C\x3A\x13\x20\x57\x29\x50\x0D\x36\x7F", 20).c_str());

    auto str = (jstring) env->CallObjectMethod(context, getPackageNameId);
    return env->GetStringUTFChars(str, 0);
}

const char *GetDeviceUniqueIdentifier(JNIEnv *env, const char *uuid) {
    jclass uuidClass = env->FindClass(/*java/util/UUID*/ StrEnc("B/TxJ=3BZ_]SFx", "\x28\x4E\x22\x19\x65\x48\x47\x2B\x36\x70\x08\x06\x0F\x3C", 14).c_str());

    auto len = strlen(uuid);

    jbyteArray myJByteArray = env->NewByteArray(len);
    env->SetByteArrayRegion(myJByteArray, 0, len, (jbyte *) uuid);

    jmethodID nameUUIDFromBytesMethod = env->GetStaticMethodID(uuidClass, /*nameUUIDFromBytes*/ StrEnc("P6LV|'0#A+zQmoat,", "\x3E\x57\x21\x33\x29\x72\x79\x67\x07\x59\x15\x3C\x2F\x16\x15\x11\x5F", 17).c_str(), /*([B)Ljava/util/UUID;*/ StrEnc("sW[\"Q[W3,7@H.vT0) xB", "\x5B\x0C\x19\x0B\x1D\x31\x36\x45\x4D\x18\x35\x3C\x47\x1A\x7B\x65\x7C\x69\x3C\x79", 20).c_str());
    jmethodID toStringMethod = env->GetMethodID(uuidClass, /*toString*/ StrEnc("2~5292eW", "\x46\x11\x66\x46\x4B\x5B\x0B\x30", 8).c_str(), /*()Ljava/lang/String;*/ StrEnc("P$BMc' #j?<:myTh_*h0", "\x78\x0D\x0E\x27\x02\x51\x41\x0C\x06\x5E\x52\x5D\x42\x2A\x20\x1A\x36\x44\x0F\x0B", 20).c_str());

    auto obj = env->CallStaticObjectMethod(uuidClass, nameUUIDFromBytesMethod, myJByteArray);
    auto str = (jstring) env->CallObjectMethod(obj, toStringMethod);
    return env->GetStringUTFChars(str, 0);
}

#include <curl/curl.h>

struct MemoryStruct {
    char *memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *) userp;

    mem->memory = (char *) realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == NULL) {
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

//#include "LicenseTools.h"

void native_Init(JNIEnv *env, jclass clazz, jobject mContext) {
    // szToast = "Modded by Kuroyama\nTelegram: @KuroHackOfficial"
    char szToast[47] = {0x28, 0x08, 0x05, 0x07, 0x08, 0x0B, 0x51, 0x11,
                        0x2C, 0x57, 0x3E, 0x26, 0x2B, 0x20, 0x38, 0x22,
                        0x28, 0x26, 0x8F, 0x57, 0x28, 0x33, 0x2C, 0x2C,
                        0x43, 0x36, 0x3C, 0x91, 0x7D, 0x5F, 0x66, 0x4E,
                        0x53, 0x48, 0x71, 0x4A, 0x4A, 0x54, 0x72, 0x4D,
                        0x4F, 0x5E, 0x56, 0x62, 0x5C, 0x63, 0xC1};

    for (unsigned int oywdm = 0, SmnWs = 0; oywdm < 47; oywdm++) {
        SmnWs = szToast[oywdm];
        SmnWs++;
        SmnWs -= oywdm;
        SmnWs++;
        SmnWs -= oywdm;
        SmnWs ^= 0x67;
        szToast[oywdm] = SmnWs;
    }

    jstring pMsg = env->NewStringUTF(szToast);





    jclass toastClass = env->FindClass(/*android/widget/Toast*/ StrEnc("eCS7dp(}Z:2f/$i/S'CO", "\x04\x2D\x37\x45\x0B\x19\x4C\x52\x2D\x53\x56\x01\x4A\x50\x46\x7B\x3C\x46\x30\x3B", 20).c_str());
    jmethodID makeTextMethod = env->GetStaticMethodID(toastClass, /*makeText*/ StrEnc("jI/4d64U", "\x07\x28\x44\x51\x30\x53\x4C\x21", 8).c_str(), /*(Landroid/content/Context;Ljava/lang/CharSequence;I)Landroid/widget/Toast;*/ StrEnc("H#L{!#l1/A\"2Z.X>f@XyZ3A*Za^XIT6&u0^zeYt%go5U[V!{X6(RXl>-!sFl9cE(l^[%f\\a2XM", "\x60\x6F\x2D\x15\x45\x51\x03\x58\x4B\x6E\x41\x5D\x34\x5A\x3D\x50\x12\x6F\x1B\x16\x34\x47\x24\x52\x2E\x5A\x12\x32\x28\x22\x57\x09\x19\x51\x30\x1D\x4A\x1A\x1C\x44\x15\x3C\x50\x24\x2E\x33\x4F\x18\x3D\x0D\x61\x7B\x14\x0D\x50\x49\x53\x1C\x2F\x08\x16\x14\x2C\x4C\x0B\x3B\x2F\x0A\x32\x33\x00\x41\x2C\x76", 74).c_str());

    jobject toastObj = env->CallStaticObjectMethod(toastClass, makeTextMethod, mContext, pMsg, 0);

    // jmethodID methodShow = env->GetMethodID(toastClass, /*show*/ StrEnc("!;N9", "\x52\x53\x21\x4E", 4).c_str(), /*()V*/ StrEnc("U*9", "\x7D\x03\x6F", 3).c_str());
    // env->CallVoidMethod(toastObj, methodShow);

    auto pkgName = GetPackageName(env, mContext);

    //StartRuntimeHook(pkgName);
}


jstring native_Check(JNIEnv *env, jclass clazz, jobject mContext, jstring mUserKey) {
    auto userKey = env->GetStringUTFChars(mUserKey, 0);

    std::string hwid = userKey;
    hwid += GetAndroidID(env, mContext);
    hwid += GetDeviceModel(env);
    hwid += GetDeviceBrand(env);

    std::string UUID = GetDeviceUniqueIdentifier(env, hwid.c_str());

    std::string errMsg;

    struct MemoryStruct chunk{};
    chunk.memory = (char *) malloc(1);
    chunk.size = 0;

    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        std::string BHATIA = OBFUSCATE ("https://anantandroid.000webhostapp.com/public/connect");
        curl_easy_setopt(curl, CURLOPT_URL ,BHATIA.c_str());

        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, /*https*/ StrEnc("!mLBO", "\x49\x19\x38\x32\x3C", 5).c_str());
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, /*Content-Type: application/x-www-form-urlencoded*/ StrEnc("@;Ls\\(KP4Qrop`b#d3094/r1cf<c<=H)AiiBG6i|Ta66s2[", "\x03\x54\x22\x07\x39\x46\x3F\x7D\x60\x28\x02\x0A\x4A\x40\x03\x53\x14\x5F\x59\x5A\x55\x5B\x1B\x5E\x0D\x49\x44\x4E\x4B\x4A\x3F\x04\x27\x06\x1B\x2F\x6A\x43\x1B\x10\x31\x0F\x55\x59\x17\x57\x3F", 47).c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        char data[4096];
        sprintf(data, /*game=PUBG&user_key=%s&serial=%s*/ StrEnc("qu2yXK,YkJyGD@ut0.u~Nb'5(:.:chK", "\x16\x14\x5F\x1C\x65\x1B\x79\x1B\x2C\x6C\x0C\x34\x21\x32\x2A\x1F\x55\x57\x48\x5B\x3D\x44\x54\x50\x5A\x53\x4F\x56\x5E\x4D\x38", 31).c_str(), userKey, UUID.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            try {
                json result = json::parse(chunk.memory);
                if (result[/*status*/ StrEnc("(>_LBm", "\x5B\x4A\x3E\x38\x37\x1E", 6).c_str()] == true) {
                    std::string token = result[/*data*/ StrEnc("fAVA", "\x02\x20\x22\x20", 4).c_str()][/*token*/ StrEnc("{>3Lr", "\x0F\x51\x58\x29\x1C", 5).c_str()].get<std::string>();
                    time_t rng = result[/*data*/ StrEnc("fAVA", "\x02\x20\x22\x20", 4).c_str()][/*rng*/ StrEnc("+n,", "\x59\x00\x4B", 3).c_str()].get<time_t>();
                    //exp_time = result[/*data*/ StrEnc("fAVA", "\x02\x20\x22\x20", 4).c_str()][/*EXP*/ StrEnc("V$1", "\x13\x7C\x61", 3).c_str()].get<std::string>();
                    if (rng + 30 > time(0)) {
                        std::string auth = /*PUBG*/ StrEnc("Q*) ", "\x01\x7F\x6B\x67", 4).c_str();;
                        auth += "-";
                        auth += userKey;
                        auth += "-";
                        auth += UUID;
                        auth += "-";
                        auth += /*Vm8Lk7Uj2JmsjCPVPVjrLa7zgfx3uz9E*/ StrEnc(OBFUSCATE("ZD$_K NtaM8Fu=n0fFyO;!Ae<H)*Gy4%"),OBFUSCATE("\x0C\x29\x1C\x13\x20\x17\x1B\x1E\x53\x07\x55\x35\x1F\x7E\x3E\x66\x36\x10\x13\x3D\x77\x40\x76\x1F\x5B\x2E\x51\x19\x32\x03\x0D\x60"), 32).c_str();
                        std::string outputAuth = Tools::CalcMD5(auth);

                        g_Token = token;
                        g_Auth = outputAuth;
                        
                        bValid = g_Token == g_Auth;
                        
                        if (bValid) {
                            pthread_t t;
                            pthread_create(&t, NULL, main_thread, NULL);
                            //pthread_create(&t, NULL, hack_thread, NULL);
                        }
                    }
                } else {
                    errMsg = result[/*reason*/ StrEnc("LW(3(c", "\x3E\x32\x49\x40\x47\x0D", 6).c_str()].get<std::string>();
                }
            } catch (json::exception &e) {
                errMsg = "{";
                errMsg += e.what();
                errMsg += "}\n{";
                errMsg += chunk.memory;
                errMsg += "}";
            }
        } else {
            errMsg = curl_easy_strerror(res);
        }
    }
    curl_easy_cleanup(curl);
    return bValid ? env->NewStringUTF(/*OK*/ StrEnc("8q", "\x77\x3A", 2).c_str()) : env->NewStringUTF(errMsg.c_str());
}


int Register2(JNIEnv *env) {
    JNINativeMethod methods[] = {{OBFUSCATE("Init"),  "(Landroid/content/Context;)V",                   (void *) native_Init},
                                 {OBFUSCATE("Check"), "(Landroid/content/Context;Ljava/lang/String;)Ljava/lang/String;", (void *) native_Check}};

    jclass clazz = env->FindClass(OBFUSCATE("com/xelahot/pubgm/Launcher"));
    if (!clazz)
        return -1;

    if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) != 0)
        return -1;

    return 0;
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    vm->GetEnv((void **) &env, JNI_VERSION_1_6);
	

    if (Register2(env) != 0)
        return -1;

    return JNI_VERSION_1_6;
}

/*!
 * @file
 * @brief This file contains implemenation of phong vertex and fragment shader.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <assert.h>
#include <math.h>

#include <student/gpu.h>
#include <student/student_shader.h>
#include <student/uniforms.h>

/// \addtogroup shader_side Úkoly v shaderech
/// @{

void phong_vertexShader(GPUVertexShaderOutput *const      output,
                        GPUVertexShaderInput const *const input,
                        GPU const                         gpu) {
  /// \todo Naimplementujte vertex shader, který transformuje vstupní vrcholy do
  /// clip-space.<br>
  /// <b>Vstupy:</b><br>
  /// Vstupní vrchol by měl v nultém atributu obsahovat pozici vrcholu ve
  /// world-space (vec3) a v prvním
  /// atributu obsahovat normálu vrcholu ve world-space (vec3).<br>
  /// <b>Výstupy:</b><br>
  /// Výstupní vrchol by měl v nultém atributu obsahovat pozici vrcholu (vec3)
  /// ve world-space a v prvním
  /// atributu obsahovat normálu vrcholu ve world-space (vec3).
  /// Výstupní vrchol obsahuje pozici a normálu vrcholu proto, že chceme počítat
  /// osvětlení ve world-space ve fragment shaderu.<br>
  /// <b>Uniformy:</b><br>
  /// Vertex shader by měl pro transformaci využít uniformní proměnné obsahující
  /// view a projekční matici.
  /// View matici čtěte z uniformní proměnné "viewMatrix" a projekční matici
  /// čtěte z uniformní proměnné "projectionMatrix".
  /// Zachovejte jména uniformních proměnných a pozice vstupních a výstupních
  /// atributů.
  /// Pokud tak neučiníte, akceptační testy selžou.<br>
  /// <br>
  /// Využijte vektorové a maticové funkce.
  /// Nepředávajte si data do shaderu pomocí globálních proměnných.
  /// Pro získání dat atributů použijte příslušné funkce vs_interpret*
  /// definované v souboru program.h.
  /// Pro získání dat uniformních proměnných použijte příslušné funkce
  /// shader_interpretUniform* definované v souboru program.h.
  /// Vrchol v clip-space by měl být zapsán do proměnné gl_Position ve výstupní
  /// struktuře.<br>
  /// <b>Seznam funkcí, které jistě použijete</b>:
  ///  - gpu_getUniformsHandle()
  ///  - getUniformLocation()
  ///  - shader_interpretUniformAsMat4()
  ///  - vs_interpretInputVertexAttributeAsVec3()
  ///  - vs_interpretOutputVertexAttributeAsVec3()
  //(void)output;
  //(void)input;
  //(void)gpu;
  
  // Inicializacia Handleru
  Uniforms const uniformHandle = gpu_getUniformsHandle(gpu);
  
  // Ulozenie vstupneho vektoru a normaly
  Vec3 const*const vertex = vs_interpretInputVertexAttributeAsVec3(gpu, input, 0);
  Vec3 const*const normal = vs_interpretInputVertexAttributeAsVec3(gpu, input, 1);
  
  // Ulozenie lokacie projectionMatrixu a viewMatrixu
  UniformLocation const projectionLoc = getUniformLocation(gpu, "projectionMatrix");
  UniformLocation const viewLoc = getUniformLocation(gpu, "viewMatrix");
  
  // Ukazatel na projectionMatrix a viewMatrix
  Mat4 const*const projection = shader_interpretUniformAsMat4(uniformHandle, projectionLoc);
  Mat4 const*const view = shader_interpretUniformAsMat4(uniformHandle, viewLoc);
  
  Mat4 mvp;
  multiply_Mat4_Mat4(&mvp, projection, view);
  
  Vec4 proj4;
  copy_Vec3Float_To_Vec4(&proj4, vertex, 1.f);
  multiply_Mat4_Vec4(&output->gl_Position, &mvp, &proj4);
  
  // Ulozenie pozicie vrcholu na nulty a pozicie normaly na prvy atribut podla zadania vystupu
  Vec3 *const out = vs_interpretOutputVertexAttributeAsVec3(gpu, output, 0);
  Vec3 *const out_normal = vs_interpretOutputVertexAttributeAsVec3(gpu, output, 1);
  
  init_Vec3(out, vertex->data[0], vertex->data[1], vertex->data[2]);
  init_Vec3(out_normal, normal->data[0], normal->data[1], normal->data[2]);
  
}

void phong_fragmentShader(GPUFragmentShaderOutput *const      output,
                          GPUFragmentShaderInput const *const input,
                          GPU const                           gpu) {
  /// \todo Naimplementujte fragment shader, který počítá phongův osvětlovací
  /// model s phongovým stínováním.<br>
  /// <b>Vstup:</b><br>
  /// Vstupní fragment by měl v nultém fragment atributu obsahovat
  /// interpolovanou pozici ve world-space a v prvním
  /// fragment atributu obsahovat interpolovanou normálu ve world-space.<br>
  /// <b>Výstup:</b><br>
  /// Barvu zapište do proměnné color ve výstupní struktuře.<br>
  /// <b>Uniformy:</b><br>
  /// Pozici kamery přečtěte z uniformní proměnné "cameraPosition" a pozici
  /// světla přečtěte z uniformní proměnné "lightPosition".
  /// Zachovejte jména uniformních proměnný.
  /// Pokud tak neučiníte, akceptační testy selžou.<br>
  /// <br>
  /// Dejte si pozor na velikost normálového vektoru, při lineární interpolaci v
  /// rasterizaci může dojít ke zkrácení.
  /// Zapište barvu do proměnné color ve výstupní struktuře.
  /// Shininess faktor nastavte na 40.f
  /// Difuzní barvu materiálu nastavte podle normály povrchu.
  /// V případě, že normála směřuje kolmo vzhůru je difuzní barva čistě bílá.
  /// V případě, že normála směřuje vodorovně nebo dolů je difuzní barva čiště zelená.
  /// Difuzní barvu spočtěte lineární interpolací zelené a bíle barvy pomocí interpolačního parameteru t.
  /// Interpolační parameter t spočtěte z y komponenty normály pomocí t = y*y (samozřejmě s ohledem na negativní čísla).
  /// Spekulární barvu materiálu nastavte na čistou bílou.
  /// Barvu světla nastavte na bílou.
  /// Nepoužívejte ambientní světlo.<br>
  /// <b>Seznam funkcí, které jistě využijete</b>:
  ///  - shader_interpretUniformAsVec3()
  ///  - fs_interpretInputAttributeAsVec3()
  //(void)output;
  //(void)input;
  //(void)gpu;
  
  // Inicializacia svetla na biele
  Vec3 white_color;
  init_Vec3(&white_color, 1.f, 1.f, 1.f);
  
  // Inicializacia farby na zelenu RGB(0, 255, 0)
  Vec3 color;
  init_Vec3(&color, 0.f, 1.f, 0.f);
  
  // Inicializacia handleru
  Uniforms const uniformHandle = gpu_getUniformsHandle(gpu);
  
  // Ulozenie lokacie lighLocation a cameraLocation
  UniformLocation const lightLocation = getUniformLocation(gpu, "lightPosition");
  UniformLocation const cameraLocation = getUniformLocation(gpu, "cameraPosition");
  
  // Ukazatel na lightLocation, cameraLocation, vektor a normalu
  Vec3 const* light = shader_interpretUniformAsVec3(uniformHandle, lightLocation);
  Vec3 const* camera = shader_interpretUniformAsVec3(uniformHandle, cameraLocation);
  Vec3 const* normal = fs_interpretInputAttributeAsVec3(gpu, input, 1);
  Vec3 const*const vertex = fs_interpretInputAttributeAsVec3(gpu, input, 0);
  
  Vec3 lightPosition;
  Vec3 cameraPosition;
  sub_Vec3(&lightPosition, light, vertex);
  sub_Vec3(&cameraPosition, camera, vertex);
  
  Vec3 normalLightPosition;
  Vec3 normalCameraPosition;
  Vec3 N;
  normalize_Vec3(&normalLightPosition, &lightPosition);
  normalize_Vec3(&normalCameraPosition, &cameraPosition);
  normalize_Vec3(&N, normal);
  
  if(N.data[1] < 0) {
  	; // Stay green
  } else if(N.data[1] > 0 && N.data[1] < 1) {
  	float t = N.data[1] * N.data[1];	// t = y*y
  	init_Vec3(&color, t, 1.f, t);
  }
  
  float scalar = dot_Vec3(&N, &normalLightPosition);
  if(scalar < 0.f)
  	init_Vec3(&color, 0.f, 0.f, 0.f);							// Mimo svetla == cierna
  else
  	multiply_Vec3_Float(&color, &color, scalar);	// Vynasobit koeficientom podla svetlosti
  
  // Part for shininess
  float shininess = 40.f;
  Vec3 reflection;
  Vec3 minusLight;
  multiply_Vec3_Float(&minusLight, &normalLightPosition, -1);
  
  reflect(&reflection, &minusLight, &N);
  
  // New scalar for us
  scalar = dot_Vec3(&normalCameraPosition, &reflection);
  if(scalar <= 0.f) {
  	; // Vypocet zbytocny, vo vysledku sa nestane nic
  } else {
  	float constant = (float)pow(scalar, shininess);
  	multiply_Vec3_Float(&white_color, &white_color, constant);
  	add_Vec3(&color, &color, &white_color);
  }
  	
  copy_Vec3Float_To_Vec4(&output->color, &color, 1.f);
  
}

/// @}

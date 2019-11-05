namespace MR {

class Shader {
    public:
        enum ShaderType {
            VERTEX_SHADER,
            FRAGMENT_SHADER
        };

        static unsigned int compileShader(Shader::ShaderType st, const char* source);
        static unsigned int linkShader(unsigned int vertexShader, unsigned int fragmentShader);
};
}

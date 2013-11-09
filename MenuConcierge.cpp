/**
 * @file MenuConcierge.cpp
 * @brief SPC�t���[�����[�N�T���v���A�v���P�[�V�����A���C���t�@�C��
 *
 * @note �{�T���v���v���O�����̍ŏI�s�ɂ���ϐ��錾�͔��ɏd�v�ł��B
 * SPC�t���[�����[�N�́A�uspcApp�v�ƌ����Œ�̕ϐ��ɑ΂��ď������s���悤�ɍ���Ă��܂��B
 */

#include "spc/spcbase.h"

/**
 * �猟�o�C�x���g�A���̌��m�C�x���g�ɔ������āAPALRO�ƊȒP�ȉ�b���s���T���v���ł��B
 *
 * �猟�o�C�x���g�Œʒm���ꂽ���O���L�����A���̌��m�C�x���g�̎��ɌĂт����܂��B
 * �܂��A�Ăт����́A �w �͂��B�x�A�w �������B �x�A�w ��߂�B �x ��3���`���̎���v��API���g�p���čs���A
 * �w ��߂�B �x�Ɠ��������ɃA�v���P�[�V�������I�����܂��B
 * �猟�o��ɁA��F�����ł��Ȃ��Ȃ����ꍇ�ɂ��A�猟�o�C�x���g�Œʒm���ꂽ���O�ŌĂт����܂��B
 *
 */
class MenuConcierge : spc::SPCBase {

  private:
    std::string face_catch_name;

  public:

    /**
     * @brief SPC�N�����Ƀt���[�����[�N�ɂ��Ăяo�����C���^�t�F�[�X�B
     *
     */
    void onInitialize() {
        long    result;

        face_catch_name = "";

        // ���b�͕K�{�ł͂���܂��񂪁A�A�v���P�[�V�����N���m�F�̂��߂ɔ��b�����Ă��܂��B
        result = speak("�T���v���N�����܂���");

        // ���̌��m���J�n
        result = startDetectMovingObject();
        if(result != 0){
            // ���bAPI�Ăяo��
            result = speak("���̌��m�J�n�͎��s���܂����A"
            "���̌��m���ł��Ȃ����߃A�v���P�[�V�������I�����܂��B");

            // �A�v���P�[�V�����I��API�Ăяo��
            result = exitComponent();
        }
    }

    /**
     * @brief �l�̊�����o�����ۂɃt���[�����[�N�ɂ��Ăяo�����C���^�t�F�[�X
     * @param name �F�������l�̓o�^��(�S�p�J�^�J�i). �l���m��̏ꍇ�͋󔒕�����
     * @param owner �F�������l��PALRO�̃I�[�i�[�ł��邩�ǂ���. �I�[�i�[�̏ꍇ��true
     *
     */
    void onFaceCatch(std::string name, bool owner) {
        long    result;
        std::string str;

        /*
         * �环�ʌ��ʂ̓o�^�����i�[����Ă���ꍇ�́A�o�^���ŌĂт����܂��B
         */
        if(name != ""){       // �o�^�҂̏ꍇ
            // ���b���e�g�ݗ���
            face_catch_name = name + "����";
            str = name + "�A����ɂ���";
        }
        else{   // ���m�̐l�̏ꍇ
            face_catch_name = "���́[";
            str = "�ǂȂ����킩��܂��񂪁A����ɂ���";
        }

        // ���bAPI�Ăяo��
        result = speak(str);
    }

    /**
     * @brief onCatchedFace��ɁA������������ۂɃt���[�����[�N�ɂ��Ăяo�����C���^�t�F�[�X
     *
     * �猟�o���s��ꂽ��ł���΁A�猟�o�Őݒ肵���Ăі���2�񔭘b���܂��B
     */
    void onFaceDrop() {
        if(face_catch_name != ""){
            long    result;

            // ���bAPI�Ăяo��
            result = speak(face_catch_name);
            result = speak("�ǂ��ł����[�H");
        }
    }

    /**
     * @brief ���̂����m�����ۂɃt���[�����[�N�ɂ��Ăяo�����C���^�t�F�[�X
     * @param speed ���̈ړ����x
     * @param direction ���m�������̂̕���
     *
     * ���̂����m������A�猟�o�Őݒ肵���Ăі����g�p���āA���̌��m��m�点�܂��B
     */
    void onMovingObjectCatch(float speed, spc::SPC_MOVING_OBJECT_DIRECTION direction)
    {
        long    result;

        if(face_catch_name != ""){
            spc::SPC_ANSWER ans_result;

            // ���bAPI�Ăяo��
            result = speak(face_catch_name);

            /*
             * ����v��(Yes/No)
             *  �w �͂��B�x�A�w �������B �x�A�w ��߂�B �x �ŉ񓚂��鎿�������
             */
            result = waitForAnswer("���A���������܂������H", ans_result);

            // ���⌋�ʊm�F
            if(result == spc::SPC_ANSWER_RECOGEND){
                switch(ans_result){
                case spc::SPC_ANSWER_YES:   // �w �͂��B�x �Ɖ񓚂��ꂽ
                    result = speak("����ς肻���ł����B");
                    break;
                case spc::SPC_ANSWER_NO:        // �w �������B�x �Ɖ񓚂��ꂽ
                    result = speak("���������Ȃ��B");
                    break;
                case spc::SPC_ANSWER_CANCEL:    // �w ��߂�B�x �Ɖ񓚂��ꂽ
                    result = speak("�킩��܂����A�A�v���P�[�V�������I�����܂��B");

                    // �A�v���P�[�V�����I��API�Ăяo��
                    result = exitComponent();
                    break;
                default:
                    break;
                }
            }
        }
    }

    /**
     * @brief SPC�I�����Ƀt���[�����[�N�ɂ��Ăяo�����C���^�t�F�[�X�B
     *
     * onInitialize�ŊJ�n�������̌��m���I�������ASPC�A�v���P�[�V�������I��������B
     */
    void onFinalize() {
        long    result;

        // onInitialize�œ��̌��m���J�n�����̂ŁA���̌��m���I������
        result = stopDetectMovingObject();

        // SPC�I��
        //exit(0);
    }

};

// SPC�A�v���P�[�V�����C���X�^���X����
MenuConcierge   spcApp;
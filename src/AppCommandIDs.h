/*
 Copyright 2013 Adobe Systems Incorporated
 Distributed under the terms of the Gnu General Public License
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
// Authors: Nicholas J. Bryan
#ifndef __APPCOMMANDIDS_H_EB481F64__
#define __APPCOMMANDIDS_H_EB481F64__

#include <map>
#include <string>
#include "JuceHeader.h"

class AppCommandIDs
{
public:
    enum ID
    {
        NewProject              = 0x3000,
        LoadProject             = 0x3001,
        SaveProject             = 0x3002,
        SaveProjectAs           = 0x3003,
        Quit                    = 0x3005,
        CloseProject            = 0x3006,
        
        Undo                    = 0x3007,
        Redo                    = 0x3008,
        
        OpenSpectrogramSettings = 0x3009,
        OpenSeparationSettings  = 0x300A,
        ExportAudio             = 0x300B,
        OpenAudioSettings       = 0x300C,
        OpenPluginListEditor    = 0x300D,
        
        OpenAboutPage           = 0x300E,
        
        
        ZoomInHorizontal        = 0x2000,
        ZoomOutHorizontal       = 0x2001,
        ZoomInVertical          = 0x2002,
        ZoomOutVertical         = 0x2003,
        ZoomIn                  = 0x2004,
        ZoomOut                 = 0x2005,
        
        Play                    = 0x4000,
        Pause                   = 0x4001,
        Stop                    = 0x4002,
        Next                    = 0x4003,
        Previous                = 0x4004,
        FastForward             = 0x4005,
        Rewind                  = 0x4006,
        Loop                    = 0x4007,
        ChangeVolume            = 0x4008,
        AutoSolo                = 0x4009,

        ArrowSelector           = 0x5000,
        BrushSelector           = 0x5001,
        TimeSelector            = 0x5002,
        FreqSelector            = 0x5003,
        BoxSelector             = 0x5004,
        ChangeBrushStrength     = 0x5005,
        SourceColor1            = 0x5006,
        SourceColor2            = 0x5007,
        EraseMode               = 0x5008,
        PaintMode               = 0x5009,
        Process                 = 0x500A,
        ClearAllMasks           = 0x500B,
        AddToMask               = 0x500D,
        TrainingSelector        = 0x500C,
        SubtractToMask          = 0x500E,
        ClearImage              = 0x500F,
        ClearTraining           = 0x5010,
        ToggleInteraction       = 0x5011,
        SoundPenSelector        = 0x5013,
        TimelineEvent           = 0x5014,
        ChangeBrushSettings     = 0x5015,
        
        // Actions add for event logging         
        
        SetSelectedTrack        = 0x5016,
        MuteTrack               = 0x5017,
        SoloTrack               = 0x5018,
        UpdatedTraining         = 0x5019,
        SoundPen                = 0x501A,
        ChangeSourceColor       = 0x501B,
        PlayLogFile             = 0x501C,
        Track                   = 0x501D,
        HorizontalScrollBarMoved= 0x501E,
        UpdateSeparationProgress= 0x501F,
        OpenSingleTrack         = 0x5020,
        OpenMultiTrack          = 0x5021,

        
    };
    
//    static std::map<String, int> createMap()
//    {
//        std::map<String, int> m;
//        m["TimelineEvent"]          = TimelineEvent;
//        m["SourceColor1"]           = SourceColor1;
//        m["SourceColor2"]           = SourceColor2;
//        m["ChangeBrushStrength"]    = ChangeBrushStrength;
//        m["ChangeBrushSettings"]    = ChangeBrushSettings;
//        m["SetSelectedTrack"]       = SetSelectedTrack;
//        m["MuteTrack"]              = MuteTrack;
//        m["SoloTrack"]              = SoloTrack;
//        m["UpdatedTraining"]        = UpdatedTraining;
//        m["SoundPen"]               = SoundPen;
//        m["ChangeSourceColor"]      = ChangeSourceColor;
//        m["AutoSolo"]               = AutoSolo;
//        m["ArrowSelector"]          = ArrowSelector;
//        m["FreqSelector"]           = FreqSelector;
//        m["TimeSelector"]           = TimeSelector;
//        m["BoxSelector"]            = BoxSelector;
//        m["TrainingSelector"]       = TrainingSelector;
//        m["SoundPenSelector"]       = SoundPenSelector;
//        m["BrushSelector"]          = BrushSelector;
//        m["Process"]                = Process;
//        m["Play"]                   = Play;
//        m["Pause"]                  = Pause;
//        m["Stop"]                   = Stop;
//        m["Previous"]               = Previous;
//        m["Next"]                   = Next;
//        m["Rewind"]                 = Rewind;
//        m["FastForward"]            = FastForward;
//        m["EraseMode"]              = EraseMode;
//        m["PaintMode"]              = PaintMode;
//        m["ZoomInHorizontal"]       = ZoomInHorizontal;
//        m["ZoomOutHorizontal"]      = ZoomOutHorizontal;
//        m["ZoomInVertical"]         = ZoomInVertical;
//        m["ZoomOutVertical"]        = ZoomOutVertical;
//        m["ZoomIn"]                 = ZoomIn;
//        m["ZoomOut"]                = ZoomOut;
//        m["Undo"]                   = Undo;
//        m["Redo"]                   = Redo;
//        m["ClearAllMasks"]          = ClearAllMasks;
//        m["AddToMask"]              = AddToMask;
//        m["ClearTraining"]          = ClearTraining;
//        m["ToggleInteraction"]      = ToggleInteraction;
//        m["ExportAudio"]            = ExportAudio;
//        m["NewProject"]             = NewProject;
//        m["PlayLogFile"]            = PlayLogFile;
//        m["HorizontalScrollBarMoved"] = HorizontalScrollBarMoved;
//        m["Track"]             = Track;
//        
//        
//        return m;
//    }
//    
//    static std::map<String, int> commandToID;
};




#endif  // __APPCOMMANDIDS_H_EB481F64__

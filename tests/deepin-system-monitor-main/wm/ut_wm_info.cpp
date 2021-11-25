/*
* Copyright (C) 2019 ~ 2021 Uniontech Software Technology Co.,Ltd.
*
* Author:     lishiqi <lishiqi@uniontech.com>
*
* Maintainer: lishiqi  <lishiqi@uniontech.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
//self
#include "wm/wm_connection.h"
#include "wm/wm_info.h"

//gtest
#include "stub.h"
#include <gtest/gtest.h>
#include "addr_pri.h"
//Qt
#include <QPoint>
#include <QDebug>
using namespace core::wm;

//*******************************tpye definition**********************************************//
enum wm_window_type_t {
    kUnknownWindowType              = -1,
    kNormalWindowType               = 0,
    kDesktopWindowType,
    kDockWindowType,
    kToolbarWindowType,
    kMenuWindowType,
    kUtilityWindowType,
    kSplashWindowType,
    kDialogWindowType,
    kDropdownMenuWindowType,
    kPopupMenuWindowType,
    kTooltipWindowType,
    kNotificationWindowType,
    kComboWindowType,
    kDNDWindowType
};

enum wm_state_t {
    kUnknownState                   = -1,
    kModalState                     = 0,
    kStickyState,
    kMaximizedVertState,
    kMaximizedHorzState,
    kShadedState,
    kSkipTaskbarState,
    kSkipPagerState,
    kHiddenState,
    kFullScreenState,
    kAboveState,
    kBelowState,
    kDemandsAttentionState,
    kFocusedState
};

enum wm_window_class_t {
    kUnknownClass                   = -1,
    kInputOutputClass               = XCB_WINDOW_CLASS_INPUT_OUTPUT,
    kInputOnlyClass                 = XCB_WINDOW_CLASS_INPUT_ONLY
};

enum wm_window_map_state_t {
    kUnknownMapState                = -1,
    kUnMappedState                  = XCB_MAP_STATE_UNMAPPED,
    kUnViewableState                = XCB_MAP_STATE_UNVIEWABLE,
    kViewableState                  = XCB_MAP_STATE_VIEWABLE
};

struct core::wm::wm_request_t {
    xcb_get_property_cookie_t           netNameCookie;
    xcb_get_property_cookie_t           nameCookie;
    xcb_get_geometry_cookie_t           geomCookie;
    xcb_translate_coordinates_cookie_t  transCoordsCookie;
    xcb_get_window_attributes_cookie_t  attrCookie;
    xcb_get_property_cookie_t           desktopCookie;
    xcb_get_property_cookie_t           windowTypeCookie;
    xcb_get_property_cookie_t           stateCookie;
    xcb_get_property_cookie_t           pidCookie;
    xcb_get_property_cookie_t           frameExtentsCookie;
    xcb_query_tree_cookie_t             treeCookie;
};

struct core::wm::wm_frame_extents_t {
    uint left;
    uint right;
    uint top;
    uint bottom;
};

struct core::wm::wm_window_ext_t {
    WMWId                           windowId;       // window id
    WMWId                           parent      {}; // window tree schema
    QList<WMWId>                    children    {};

    pid_t                           pid;            // _NET_WM_PID
    int                             desktop     {}; // desktop
    enum wm_window_class_t          wclass      {}; // class: InputOutput || InputOnly && win attrs: XCB_WINDOW_CLASS_INPUT_OUTPUT || XCB_WINDOW_CLASS_INPUT_ONLY
    enum wm_window_map_state_t      map_state   {}; // map state: unmapped || unviewable || viewable && win attrs: XCB_MAP_STATE_UNMAPPED || XCB_MAP_STATE_UNVIEWABLE || XCB_MAP_STATE_VIEWABLE
    QList<enum wm_state_t>          states      {}; // window states && // _NET_WM_STATE
    QList<enum wm_window_type_t>    types       {}; // window types (in order of preference) && _NET_WM_WINDOW_TYPE
    struct wm_frame_extents_t       extents     {}; // window extents && //_NET_FRAME_EXTENTS
    QRect                           rect        {}; // geometry

    std::unique_ptr<wm_request_t>   request;
};

// tree schema in bottom to top stacking order
struct core::wm::wm_tree_t {
    struct wm_window_ext_t         *root;  // root window
    std::map<WMWId, WMWindowExt>    cache; // [windowId : window extended info] mapping
};
//*******************************tpye definition**********************************************//

//*******************************stub begin**********************************************//
ACCESS_PRIVATE_FUN(WMInfo, void(void), findDockWindows);

void stub_WMInfo_findDockWindows(){
    return;
}
//*******************************stub end**********************************************//
class UT_WMInfo: public ::testing::Test
{
public:
    UT_WMInfo() : m_tester(nullptr) {}

public:
    virtual void SetUp()
    {
        auto WMInfo_findDockWindows= get_private_fun::WMInfofindDockWindows();
        Stub b;
        b.set(WMInfo_findDockWindows,stub_WMInfo_findDockWindows);
        m_tester = new WMInfo();
    }

    virtual void TearDown()
    {
        if(m_tester){
            delete m_tester;
            m_tester=nullptr;
        }
    }

protected:
    WMInfo *m_tester;
};

TEST_F(UT_WMInfo, initTest)
{

}

TEST_F(UT_WMInfo, test_selectWindow_001)
{

}

TEST_F(UT_WMInfo, test_getRootWindow_001)
{
    m_tester->getRootWindow();
}

TEST_F(UT_WMInfo, test_isCursorHoveringDocks_001)
{
    QPoint pos(0,0);
    bool flag = m_tester->isCursorHoveringDocks(pos);
    EXPECT_FALSE(flag);
}

TEST_F(UT_WMInfo, test_getHoveredByWindowList_001)
{


}

TEST_F(UT_WMInfo, test_buildWindowTreeSchema_001)
{
    m_tester->buildWindowTreeSchema();
}

TEST_F(UT_WMInfo, test_findDockWindows_001)
{

}

TEST_F(UT_WMInfo, test_initAtomCache_001)
{
    WMConnection *connection = new WMConnection;
    auto *conn = connection->xcb_connection();
    m_tester->initAtomCache(conn);
    delete connection;
}

TEST_F(UT_WMInfo, test_getAtom_001)
{

}

TEST_F(UT_WMInfo, test_getAtomName_001)
{
    xcb_atom_t atom = XCB_ATOM_NONE;
    WMConnection *connection = new WMConnection;
    auto *conn = connection->xcb_connection();
    m_tester->getAtomName(conn,atom);
    delete connection;
}

TEST_F(UT_WMInfo, test_requestWindowExtInfo_001)
{
    xcb_window_t window = m_tester->getRootWindow();
    WMConnection *connection = new WMConnection;
    auto *conn = connection->xcb_connection();
    m_tester->requestWindowExtInfo(conn,window);
    delete connection;
}